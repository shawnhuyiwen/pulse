set( _cur_dir cpp/io/protobuf)

set( IO_PB_PULSE_FILES ${_cur_dir}/PBPulse.h
                       ${_cur_dir}/PBPulse.cpp
                       ${_cur_dir}/PBPulseEnums.cpp
                       ${_cur_dir}/PBPulseConfiguration.h
                       ${_cur_dir}/PBPulseConfiguration.cpp
                       ${_cur_dir}/PBPulseEnvironment.h
                       ${_cur_dir}/PBPulseEnvironment.cpp
                       ${_cur_dir}/PBPulseEquipment.h
                       ${_cur_dir}/PBPulseEquipment.cpp
                       ${_cur_dir}/PBPulsePhysiology.h
                       ${_cur_dir}/PBPulsePhysiology.cpp
                       ${_cur_dir}/PBPulseState.h
                       ${_cur_dir}/PBPulseState.cpp
                       )
source_group("IO\\Protobuf" FILES ${IO_PB_PULSE_FILES})
list(APPEND SOURCE ${IO_PB_PULSE_FILES})
