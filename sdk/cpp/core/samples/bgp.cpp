//
    //  bgp.cpp
//  ydk
//
//  Created by Manu Radhakrishna on 7/25/16.
//  Copyright © 2016 cisco. All rights reserved.
//



#include <iostream>
#include <memory>
#include "../src/path_api.hpp"


void print_paths(ydk::path::SchemaNode & sn)
{
    std::cout << sn.get_path() << std::endl;
    for(auto const& p : sn.get_children())
        print_paths(*p);
}

void test_bgp_create()
{
    ydk::path::Repository repo{};
    ydk::path::NetconfSession session{repo,"127.0.0.1", "admin", "admin",  12022};
    ydk::path::RootSchemaNode& schema = session.get_root_schema();
    print_paths(schema);

    auto & bgp = schema.create_datanode("openconfig-bgp:bgp", "");

    // get the root
    // std::unique_ptr<const ydk::path::DataNode> data_root{bgp->get_root()};

    bgp.create_datanode("global/config/as", "65172");


    auto & l3vpn_ipv4_unicast = bgp.create_datanode("global/afi-safis/afi-safi[afi-safi-name='openconfig-bgp-types:L3VPN_IPV4_UNICAST']", "");
    l3vpn_ipv4_unicast.create_datanode("config/afi-safi-name", "openconfig-bgp-types:L3VPN_IPV4_UNICAST");


    // set the enable flag
    l3vpn_ipv4_unicast.create_datanode("config/enabled","true");

    // bgp/neighbors/neighbor
    auto & neighbor = bgp.create_datanode("neighbors/neighbor[neighbor-address='172.16.255.2']", "");
    neighbor.create_datanode("config/neighbor-address", "172.16.255.2");
    neighbor.create_datanode("config/peer-as","65172");


    // bgp/neighbors/neighbor/afi-safis/afi-safi
    auto & neighbor_af = neighbor.create_datanode("afi-safis/afi-safi[afi-safi-name='openconfig-bgp-types:L3VPN_IPV4_UNICAST']", "");
    neighbor_af.create_datanode("config/afi-safi-name" , "openconfig-bgp-types:L3VPN_IPV4_UNICAST");
    neighbor_af.create_datanode("config/enabled","true");

    ydk::path::Codec s {};
    auto xml = s.encode(bgp, ydk::EncodingFormat::XML, true);
    // auto json = s.encode(bgp, ydk::path::Codec::Format::JSON, true);

    std::cout << "Testing encoding" << std::endl;

    std::cout << "*********************************************************" << std::endl;
    std::cout << "*********************************************************" << std::endl;
    std::cout <<  xml << std::endl;
    std::cout << "*********************************************************" << std::endl;
    std::cout << "*********************************************************" << std::endl;
    // std::cout <<  json << std::endl;
    std::cout << "*********************************************************" << std::endl;
    std::cout << "*********************************************************" << std::endl;

    std::cout << "Testing decoding" << std::endl;

    // todo enable after fixing bugs

    //codec service bugs
    auto new_bgp = s.decode(schema, xml, ydk::EncodingFormat::XML);
    if (new_bgp) {
        std::cout << "deserialized successfully" << std::endl;
    }

    auto new_xml = s.encode(*new_bgp, ydk::EncodingFormat::XML, true);
    std::cout << "*********************************************************" << std::endl;
    std::cout << "*********************************************************" << std::endl;
    std::cout <<  new_xml << std::endl;
    std::cout << "*********************************************************" << std::endl;
    std::cout << "*********************************************************" << std::endl;


    // TODO fix rpc
    std::shared_ptr<ydk::path::Rpc> create_rpc { schema.create_rpc("ydk:create") };
    create_rpc->get_input_node().create_datanode("entity", xml);

    // call create
    (*create_rpc)(session);
}

//void test_read(ydk::path::Session *session, ydk::SchemaNode* schema)
//{
//   const char *bgp_xml = "\
//         <bgp xmlns=\"http://openconfig.net/yang/bgp\">\
//             <global>\
//              <config>\
//               <as>65172</as>\
//              </config>\
//             <afi-safi>\
//              <afi-safi>\
//                <afi-safi-name>l3vpn-ipv4-unicast</afi-safi-name>\
//                <config>\
//                  <afi-safi-name>l3vpn-ipv4-unicast</afi-safi-name>\
//                  <enabled>true</enabled>\
//                 </config>\
//              </afi-safi>\
//            </afi-safis>\
//            </global>
////  <neighbors>
////    <neighbor>
////      <neighbor-address>172.16.255.2</neighbor-address>
////      <config>
////        <neighbor-address>172.16.255.2</neighbor-address>
////        <peer-as>65172</peer-as>
////      </config>
////      <afi-safis>
////        <afi-safi>
////          <afi-safi-name>l3vpn-ipv4-unicast</afi-safi-name>
////          <config>
////            <afi-safi-name>l3vpn-ipv4-unicast</afi-safi-name>
////            <enabled>true</enabled>
////          </config>
////        </afi-safi>
////      </afi-safis>
////    </neighbor>
////  </neighbors>
////</bgp>";
//}


int main() {

    //create
    test_bgp_create();

    //begin read

    return 0;
}
