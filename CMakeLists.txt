##################################
# Energy object class 0x4E & 0x4F#
##################################
opener_add_cip_object( ElectricalEnergyObject "Electrical Energy Object (class 0x4F)")
#######################################
# Add common includes                 #
#######################################
opener_common_includes()

#######################################
# Add platform-specific includes      #
#######################################
opener_platform_support("INCLUDES")

##################################
# Set source for library         #
##################################
set( ELECTRICAL_ENERGY_OBJECT_SOURCE cipelectricalenergy.c )

include_directories(${OpENer_ADD_CIP_OBJECTS_INCLUDES})

##################################
# Create library		 #
##################################
add_library( ELECTRICAL_ENERGY_OBJECT ${ELECTRICAL_ENERGY_OBJECT_SOURCE})

string(STRIP "${OpENer_ADD_CIP_OBJECTS} ELECTRICAL_ENERGY_OBJECT" OpENer_TRIMMED_ADD_CIP_OBJECTS)

set(OpENer_ADD_CIP_OBJECTS "${OpENer_TRIMMED_ADD_CIP_OBJECTS}" CACHE INTERNAL STRING )
target_link_libraries( ELECTRICAL_ENERGY_OBJECT CIP m)


