cmake_minimum_required (VERSION 3.8)

project(requester VERSION 1.0 LANGUAGES C)

# MSVC hot reload.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

find_package(OpenSSL REQUIRED)

list(APPEND CORE_SOURCE_FILES
    "src/core/core.h"
    "src/core/http_default.h"
    "src/core/config.h"

    "src/core/utils/utils.h"
    "src/core/utils/arrays.h"
    "src/core/utils/data_types_aliases.h"
    "src/core/utils/float_string.c"
    "src/core/utils/float_string.h"
    "src/core/utils/int_string.c"
    "src/core/utils/int_string.h"
    "src/core/utils/print_log.c"
    "src/core/utils/print_log.h"
    "src/core/utils/pointers.h"
    "src/core/utils/strings.h"
    "src/core/utils/strings.c"
    "src/core/utils/math.h"

    "src/core/network/network.h"
    "src/core/network/client_socket.c"
    "src/core/network/client_socket.h"
    "src/core/network/secure_client_layer.c"
    "src/core/network/secure_client_layer.h"

    "src/core/structs/structs.h"
    "src/core/structs/http_content.c"
    "src/core/structs/http_content.h"
    "src/core/structs/http_headers.c"
    "src/core/structs/http_headers.h"
    "src/core/structs/http_method.c"
    "src/core/structs/http_method.h"
    "src/core/structs/http_method_text.h"
    "src/core/structs/http_request.c"
    "src/core/structs/http_request.h"
    "src/core/structs/http_response.c"
    "src/core/structs/http_response.h"
    "src/core/structs/http_status.c"
    "src/core/structs/http_status.h"
    "src/core/structs/http_status_text.h"
    "src/core/structs/http_version.c"
    "src/core/structs/http_version.h"
    "src/core/structs/http_version_text.h"
    "src/core/structs/mime_type.c"
    "src/core/structs/mime_type.h"
    "src/core/structs/mime_type_text.h"
    "src/core/structs/query_params.c"
    "src/core/structs/query_params.h"
    "src/core/structs/string_map.c"
    "src/core/structs/string_map.h"
    "src/core/structs/url.c"
    "src/core/structs/url.h"
    "src/core/structs/url_protocol.c"
    "src/core/structs/url_protocol.h"
    "src/core/structs/url_protocol_text.h"
)

if (WIN32)
    list(APPEND CORE_SOURCE_FILES
        "src/core/network/client_socket_windows.c"
    )
else()
    list(APPEND CORE_SOURCE_FILES
        "src/core/network/client_socket_unix.c"
    )
endif()

list(APPEND CLIENT_SOURCE_FILES
    "src/client/client.h"
    "src/client/config.h"

    "src/client/http_client.h"
    "src/client/http_client.c"
    "src/client/http_client_abstractions.c"
    "src/client/http_client_abstractions.h"
)

list(APPEND HELPERS_SOURCE_FILES
    "src/helpers/helpers.h"

    "src/helpers/user_agents.h"
)

list(APPEND SOURCE_FILES
    "src/requester.h"

    ${CORE_SOURCE_FILES}
    ${CLIENT_SOURCE_FILES}
    ${HELPERS_SOURCE_FILES}
)

# Add the library as a dynamic library, to avoid this, define the
# REQUESTER_BUILD_STATIC variable.

if (NOT DEFINED REQUESTER_BUILD_STATIC)
    add_library (requester ${SOURCE_FILES})
else()
    add_library (requester STATIC ${SOURCE_FILES})
endif()

target_include_directories(requester PUBLIC "src" "src/core")

target_link_libraries(requester PRIVATE OpenSSL::SSL)

if (WIN32)
    target_link_libraries(requester PRIVATE ws2_32)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET requester PROPERTY C_STANDARD 90)
endif()

# Build examples.
#
# To avoid building examples by default, define the REQUESTER_DONT_BUILD_EXAMPLES
# variable.

if(NOT DEFINED REQUESTER_DONT_BUILD_EXAMPLES)
    
    add_subdirectory(examples)

endif()
