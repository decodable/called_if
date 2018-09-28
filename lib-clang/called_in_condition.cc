#include "called_if.h"
#include <clang-c/Index.h>

using namespace std;

namespace codible {

enum CXChildVisitResult match_call_expr(CXCursor c, CXCursor parent,
                                        CXClientData client_data);

} // namespace codible
