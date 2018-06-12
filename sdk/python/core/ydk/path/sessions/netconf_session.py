#  ----------------------------------------------------------------
# Copyright 2017 Cisco Systems
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ------------------------------------------------------------------

"""netconf_session.py
NetconfSession Python wrapper.
"""

from ydk.path import RootSchemaNode
from ydk.ext.path import NetconfSession as _NetconfSession
from ydk.errors.error_handler import handle_runtime_error as _handle_error


class NetconfSession(_NetconfSession):
    """ Python wrapper for NetconfSession
    """

    def __init__(self, address, username, password, port=830, protocol="ssh",
                       on_demand=True, common_cache=False, timeout=None, repo=None):

        if timeout is None:
            timeout = -1

        if repo is None:
            with _handle_error():
                super(NetconfSession, self).__init__(address, username, password,
                                                     port, protocol, on_demand,
                                                     common_cache, timeout)
        else:
            with _handle_error():
                super(NetconfSession, self).__init__(repo, address, username,
                                                     password, port, protocol,
                                                     on_demand, timeout)

    def get_root_schema(self):
        with _handle_error():
            root_schema_node = super(NetconfSession, self).get_root_schema()
            return RootSchemaNode(root_schema_node)

    def invoke(self, rpc):
        with _handle_error():
            return super(NetconfSession, self).invoke(rpc)

    def get_capabilities(self):
        with _handle_error():
            return super(NetconfSession, self).get_capabilities()
