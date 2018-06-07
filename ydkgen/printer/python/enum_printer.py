#  ----------------------------------------------------------------
# Copyright 2016 Cisco Systems
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
"""
 enum_printer.py

 YANG model driven API, python emitter.

"""


from ydkgen.api_model import Enum
from ydkgen.printer.meta_data_util import get_enum_class_docstring

class EnumPrinter(object):

    def __init__(self, ctx):
        self.ctx = ctx

    def print_enum(self, enum_class, no_meta_assign):
        assert isinstance(enum_class, Enum)
        self._print_enum_header(enum_class)
        self._print_enum_body(enum_class, no_meta_assign)
        self._print_enum_trailer()

    def _print_enum_header(self, enum_class):
        self.ctx.writeln('class %s(Enum):' % enum_class.name)
        self.ctx.lvl_inc()

    def _print_enum_body(self, enum_class, no_meta_assign):
        self._print_enum_docstring(enum_class)
        self._print_leaf_name_lookup_table(enum_class)
        self._print_enum_literals(enum_class)

    def _print_enum_docstring(self, enum_class):
        self.ctx.writeln('"""')
        enumz_docstring = get_enum_class_docstring(enum_class, 'py')
        if len(enumz_docstring):
            for line in enumz_docstring.split('\n'):
                if line.strip() != '':
                    self.ctx.writeln(line)
                    self.ctx.bline()
        self.ctx.writeln('"""')
        self.ctx.bline()

    def _print_leaf_name_lookup_table(self, enum_class):
        line = '_enumerator_name_lookup_table = {'
        for enum_literal in enum_class.literals:
            key = enum_literal.stmt.arg
            value = enum_literal.name
            line = "%s'%s': '%s', " % (line, key, value)
        line = '%s}' % line
        self.ctx.writeln(line)
        self.ctx.bline()

    def _print_enum_literals(self, enum_class):
        for enum_literal in enum_class.literals:
            self._print_enum_literal(enum_literal)

    def _print_enum_literal(self, enum_literal):
        name = enum_literal.name
        value = enum_literal.value
        self.ctx.writeln('%s = Enum.YLeaf(%s, "%s")' % (name, value, enum_literal.stmt.arg))
        self.ctx.bline()

    def _print_enum_trailer(self):
        self.ctx.lvl_dec()
        self.ctx.bline()
