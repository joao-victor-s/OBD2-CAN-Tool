#pragma once

#include "CANManager.h"
#include <iostream>
#include <cstring>
#include <map>
using namespace std;

#define OBD_TIMEOUT 2000 

//Responsabilidade: Implementar funcionalidades especificas do protocolo OBD2,
// utilizando a comunicacao CAN fornecida pelo CANManager.


enum PID {
    PIDS_SUPPORT_01_20 = 0x00,
    MONITOR_STATUS_SINCE_DTCS_CLEARED = 0x01,
    FREEZE_DTC = 0x02,
    FUEL_SYSTEM_STATUS = 0x03,
    CALCULATED_ENGINE_LOAD = 0x04,
    ENGINE_COOLANT_TEMPERATURE = 0x05,
    SHORT_TERM_FUEL_TRIM_BANK_1 = 0x06,
    LONG_TERM_FUEL_TRIM_BANK_1 = 0x07,
    SHORT_TERM_FUEL_TRIM_BANK_2 = 0x08,
    LONG_TERM_FUEL_TRIM_BANK_2 = 0x09,
    FUEL_PRESSURE = 0x0a,
    INTAKE_MANIFOLD_ABSOLUTE_PRESSURE = 0x0b,
    ENGINE_RPM = 0x0c,
    VEHICLE_SPEED = 0x0d,
    TIMING_ADVANCE = 0x0e,
    AIR_INTAKE_TEMPERATURE = 0x0f,
    MAF_AIR_FLOW_RATE = 0x10,
    THROTTLE_POSITION = 0x11,
    COMMANDED_SECONDARY_AIR_STATUS = 0x12,
    OXYGEN_SENSORS_PRESENT_IN_2_BANKS = 0x13,
    OXYGEN_SENSOR_1_SHORT_TERM_FUEL_TRIM = 0x14,
    OXYGEN_SENSOR_2_SHORT_TERM_FUEL_TRIM = 0x15,
    OXYGEN_SENSOR_3_SHORT_TERM_FUEL_TRIM = 0x16,
    OXYGEN_SENSOR_4_SHORT_TERM_FUEL_TRIM = 0x17,
    OXYGEN_SENSOR_5_SHORT_TERM_FUEL_TRIM = 0x18,
    OXYGEN_SENSOR_6_SHORT_TERM_FUEL_TRIM = 0x19,
    OXYGEN_SENSOR_7_SHORT_TERM_FUEL_TRIM = 0x1a,
    OXYGEN_SENSOR_8_SHORT_TERM_FUEL_TRIM = 0x1b,
    OBD_STANDARDS_THIS_VEHICLE_CONFORMS_TO = 0x1c,
    OXYGEN_SENSORS_PRESENT_IN_4_BANKS = 0x1d,
    AUXILIARY_INPUT_STATUS = 0x1e,
    RUN_TIME_SINCE_ENGINE_START = 0x1f,

    PIDS_SUPPORT_21_40 = 0x20,
    DISTANCE_TRAVELED_WITH_MIL_ON = 0x21,
    FUEL_RAIL_PRESSURE = 0x22,
    FUEL_RAIL_GAUGE_PRESSURE = 0x23,
    OXYGEN_SENSOR_1_FUEL_AIR_EQUIVALENCE_RATIO = 0x24,
    OXYGEN_SENSOR_2_FUEL_AIR_EQUIVALENCE_RATIO = 0x25,
    OXYGEN_SENSOR_3_FUEL_AIR_EQUIVALENCE_RATIO = 0x26,
    OXYGEN_SENSOR_4_FUEL_AIR_EQUIVALENCE_RATIO = 0x27,
    OXYGEN_SENSOR_5_FUEL_AIR_EQUIVALENCE_RATIO = 0x28,
    OXYGEN_SENSOR_6_FUEL_AIR_EQUIVALENCE_RATIO = 0x29,
    OXYGEN_SENSOR_7_FUEL_AIR_EQUIVALENCE_RATIO = 0x2a,
    OXYGEN_SENSOR_8_FUEL_AIR_EQUIVALENCE_RATIO = 0x2b,
    COMMANDED_EGR = 0x2c,
    EGR_ERROR = 0x2d,
    COMMANDED_EVAPORATIVE_PURGE = 0x2e,
    FUEL_TANK_LEVEL_INPUT = 0x2f,
    WARM_UPS_SINCE_CODES_CLEARED = 0x30,
    DISTANCE_TRAVELED_SINCE_CODES_CLEARED = 0x31,
    EVAP_SYSTEM_VAPOR_PRESSURE = 0x32,
    ABSOLULTE_BAROMETRIC_PRESSURE = 0x33,
    /*OXYGEN_SENSOR_1_FUEL_AIR_EQUIVALENCE_RATIO        = 0x34,
      OXYGEN_SENSOR_2_FUEL_AIR_EQUIVALENCE_RATIO        = 0x35,
      OXYGEN_SENSOR_3_FUEL_AIR_EQUIVALENCE_RATIO        = 0x36,
      OXYGEN_SENSOR_4_FUEL_AIR_EQUIVALENCE_RATIO        = 0x37,
      OXYGEN_SENSOR_5_FUEL_AIR_EQUIVALENCE_RATIO        = 0x38,
      OXYGEN_SENSOR_6_FUEL_AIR_EQUIVALENCE_RATIO        = 0x39,
      OXYGEN_SENSOR_7_FUEL_AIR_EQUIVALENCE_RATIO        = 0x3a,
      OXYGEN_SENSOR_8_FUEL_AIR_EQUIVALENCE_RATIO        = 0x3b,*/
    CATALYST_TEMPERATURE_BANK_1_SENSOR_1 = 0x3c,
    CATALYST_TEMPERATURE_BANK_2_SENSOR_1 = 0x3d,
    CATALYST_TEMPERATURE_BANK_1_SENSOR_2 = 0x3e,
    CATALYST_TEMPERATURE_BANK_2_SENSOR_2 = 0x3f,

    PIDS_SUPPORT_41_60 = 0x40,
    MONITOR_STATUS_THIS_DRIVE_CYCLE = 0x41,
    CONTROL_MODULE_VOLTAGE = 0x42,
    ABSOLUTE_LOAD_VALUE = 0x43,
    FUEL_AIR_COMMANDED_EQUIVALENCE_RATE = 0x44,
    RELATIVE_THROTTLE_POSITION = 0x45,
    AMBIENT_AIR_TEMPERATURE = 0x46,
    ABSOLUTE_THROTTLE_POSITION_B = 0x47,
    ABSOLUTE_THROTTLE_POSITION_C = 0x48,
    ABSOLUTE_THROTTLE_POSITION_D = 0x49,
    ABSOLUTE_THROTTLE_POSITION_E = 0x4a,
    ABSOLUTE_THROTTLE_POSITION_F = 0x4b,
    COMMANDED_THROTTLE_ACTUATOR = 0x4c,
    TIME_RUN_WITH_MIL_ON = 0x4d,
    TIME_SINCE_TROUBLE_CODES_CLEARED = 0x4e,
   
    FUEL_TYPE = 0x51,
    ETHANOL_FUEL_PERCENTAGE = 0x52,
    ABSOLUTE_EVAP_SYSTEM_VAPOR_PRESSURE = 0x53,
    
    FUEL_RAIL_ABSOLUTE_PRESSURE = 0x59,
    RELATIVE_ACCELERATOR_PEDAL_POSITTION = 0x5a,
    HYBRID_BATTERY_PACK_REMAINING_LIFE = 0x5b,
    ENGINE_OIL_TEMPERATURE = 0x5c,
    FUEL_INJECTION_TIMING = 0x5d,
    ENGINE_FUEL_RATE = 0x5e,
    EMISSION_REQUIREMENT_TO_WHICH_VEHICLE_IS_DESIGNED = 0x5f,

};


struct PIDInfo {
    string name;  // Nome do PID
    string unit;  // Unidade associada ao PID
};


extern const PIDInfo pidData[];

class OBDReader
{
private:
    CANManager* canManager;                         // Instancia do gerenciador CAN para comunicacao
    int timeout;                                    // Timeout para operacoes CAN
    static const map<PID, PIDInfo> pidMap;          // Mapa para associar PIDs as suas informações
                                                    //static -> imutavel em todas as instancias da classe
   
    
    bool isValidPID(PID pid);                       // Validacao de PIDs

public:
	OBDReader(CANManager * can, int time = OBD_TIMEOUT);
	virtual ~OBDReader();

    // sobrecarga de metodos para acessar parametros
    string getPIDName(PID pid) const;
    string getPIDName(int pidValue) const;          
    string getPIDUnit(PID pid) const;
    string getPIDUnit(int pidValue) const;

    float pidRead(PID pid);
    void begin(CANManager * can, int time = OBD_TIMEOUT);


    string vinRead(CANManager* can, int time = OBD_TIMEOUT);               //Le o numero de identificação do veiculo (VIN - Vehicle Identification Number).
    //string ecuNameRead();           //Le o nome da ECU (Electronic Control Unit).
};

