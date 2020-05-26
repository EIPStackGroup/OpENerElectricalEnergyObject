######################################################
#                                                    #
# Copyright (c) 2020, Rockwell Automation, Inc.      #
# All rights reserved.                               #
#                                                    #
######################################################

project( CIP_ELECTRICAL_ENERGY_OBJECT LANGUAGES C VERSION 3.0) 

opener_add_cip_object( ${PROJECT_NAME} "Electrical Energy Object (class 0x4F)")

#######################################
# Add common includes                 #
#######################################
opener_common_includes()

#######################################
# Add platform-specific includes      #
#######################################
opener_platform_support("INCLUDES")

include_directories(${CIP_BASE_ENERGY_OBJECT_SOURCE_DIR})

############################
# Conditions
#############################
if( OPENER_MESSAGE_DATA_REPLY_BUFFER LESS 250 )
  message(STATUS "The Electrical Energy Object needs a larger reply message buffer - value set to 250")
  set( OPENER_MESSAGE_DATA_REPLY_BUFFER "250" CACHE STRING "The number of bytes used for the reply message buffer" FORCE)
endif()

##################################
# Set source for library         #
##################################
set( ${PROJECT_NAME}_SOURCE cipelectricalenergy.c )

include_directories(${${PROJECT_NAME}_SOURCE_DIR})

##################################
# Create library		 #
##################################
add_library( ${PROJECT_NAME} ${${PROJECT_NAME}_SOURCE})

##################################
# Add object to includes         #
##################################
string(STRIP "${OpENer_ADD_CIP_OBJECTS} ${PROJECT_NAME}" OpENer_TRIMMED_ADD_CIP_OBJECTS)
set(OpENer_ADD_CIP_OBJECTS "${OpENer_TRIMMED_ADD_CIP_OBJECTS}" CACHE INTERNAL STRING )

target_link_libraries( ${PROJECT_NAME} CIP m)


