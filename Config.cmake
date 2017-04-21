################################
# WORMHOLE COMPILATION PARTS
#################################

option(WH_APPS "Enable the compilation of WH apps, including einstein" ON)
option(WH_EXAMPLES "Enable the compilation of the examples" ON)
option(WH_SSLCERTS "Enable the autogeneration of certs" ON)

################################
# WORMHOLE CONFIG
#################################

option(EINSTEIN_DEBUG "Enable Einstein debug text" ON)
option(WORMS_DEBUG "Enable Einstein debug text" ON)

#################################
# CONFIG FILES
#################################
#Config file
#configure_file (
#  "${PROJECT_SOURCE_DIR}/include/config.hpp.in"
#  "${PROJECT_BINARY_DIR}/include/config.hpp"
#  )
configure_file (
  "${PROJECT_SOURCE_DIR}/include/wh_config.h.in"
  "${PROJECT_SOURCE_DIR}/include/wh_config.h"
  )

