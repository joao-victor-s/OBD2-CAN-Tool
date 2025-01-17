#include "OBDReader.h"
#include <cstdint>



const map<PID, PIDInfo> OBDReader::pidMap = {
    { PID::PIDS_SUPPORT_01_20, { "PIDs supported [01 - 20]", "" } },
    { PID::MONITOR_STATUS_SINCE_DTCS_CLEARED, { "Monitor status since DTCs cleared", "" } },
    { PID::FREEZE_DTC, { "Freeze DTC", "" } },
    { PID::FUEL_SYSTEM_STATUS, { "Fuel system status", "" } },
    { PID::CALCULATED_ENGINE_LOAD, { "Calculated engine load", "%" } },
    { PID::ENGINE_COOLANT_TEMPERATURE, { "Engine coolant temperature", "°C" } },
    { PID::SHORT_TERM_FUEL_TRIM_BANK_1, { "Short term fuel trim — Bank 1", "%" } },
    { PID::LONG_TERM_FUEL_TRIM_BANK_1, { "Long term fuel trim — Bank 1", "%" } },
    { PID::SHORT_TERM_FUEL_TRIM_BANK_2, { "Short term fuel trim — Bank 2", "%" } },
    { PID::LONG_TERM_FUEL_TRIM_BANK_2, { "Long term fuel trim — Bank 2", "%" } },
    { PID::FUEL_PRESSURE, { "Fuel pressure", "kPa" } },
    { PID::INTAKE_MANIFOLD_ABSOLUTE_PRESSURE, { "Intake manifold absolute pressure", "kPa" } },
    { PID::ENGINE_RPM, { "Engine RPM", "rpm" } },
    { PID::VEHICLE_SPEED, { "Vehicle speed", "km/h" } },
    { PID::TIMING_ADVANCE, { "Timing advance", "° before TDC" } },
    { PID::AIR_INTAKE_TEMPERATURE, { "Intake air temperature", "°C" } },
    { PID::MAF_AIR_FLOW_RATE, { "MAF air flow rate", "grams/sec" } },
    { PID::THROTTLE_POSITION, { "Throttle position", "%" } },
    { PID::COMMANDED_SECONDARY_AIR_STATUS, { "Commanded secondary air status", "" } },
    { PID::OXYGEN_SENSORS_PRESENT_IN_2_BANKS, { "Oxygen sensors present (in 2 banks)", "" } },
    { PID::OXYGEN_SENSOR_1_SHORT_TERM_FUEL_TRIM, { "Oxygen Sensor 1 - Short term fuel trim", "%" } },
    { PID::OXYGEN_SENSOR_2_SHORT_TERM_FUEL_TRIM, { "Oxygen Sensor 2 - Short term fuel trim", "%" } },
    { PID::OXYGEN_SENSOR_3_SHORT_TERM_FUEL_TRIM, { "Oxygen Sensor 3 - Short term fuel trim", "%" } },
    { PID::OXYGEN_SENSOR_4_SHORT_TERM_FUEL_TRIM, { "Oxygen Sensor 4 - Short term fuel trim", "%" } },
    { PID::OXYGEN_SENSOR_5_SHORT_TERM_FUEL_TRIM, { "Oxygen Sensor 5 - Short term fuel trim", "%" } },
    { PID::OXYGEN_SENSOR_6_SHORT_TERM_FUEL_TRIM, { "Oxygen Sensor 6 - Short term fuel trim", "%" } },
    { PID::OXYGEN_SENSOR_7_SHORT_TERM_FUEL_TRIM, { "Oxygen Sensor 7 - Short term fuel trim", "%" } },
    { PID::OXYGEN_SENSOR_8_SHORT_TERM_FUEL_TRIM, { "Oxygen Sensor 8 - Short term fuel trim", "%" } },
    { PID::OBD_STANDARDS_THIS_VEHICLE_CONFORMS_TO, { "OBD standards this vehicle conforms to", "" } },
    { PID::OXYGEN_SENSORS_PRESENT_IN_4_BANKS, { "Oxygen sensors present (in 4 banks)", "" } },
    { PID::AUXILIARY_INPUT_STATUS, { "Auxiliary input status", "" } },
    { PID::RUN_TIME_SINCE_ENGINE_START, { "Run time since engine start", "seconds" } },
    { PID::PIDS_SUPPORT_21_40, { "PIDs supported [21 - 40]", "" } },
    { PID::DISTANCE_TRAVELED_WITH_MIL_ON, { "Distance traveled with MIL on", "km" } },
    { PID::FUEL_RAIL_PRESSURE, { "Fuel Rail Pressure", "kPa" } },
    { PID::FUEL_RAIL_GAUGE_PRESSURE, { "Fuel Rail Gauge Pressure", "kPa" } },
    { PID::OXYGEN_SENSOR_1_FUEL_AIR_EQUIVALENCE_RATIO, { "Oxygen Sensor 1 - Fuel-Air Equivalence Ratio", "ratio" } },
    { PID::OXYGEN_SENSOR_2_FUEL_AIR_EQUIVALENCE_RATIO, { "Oxygen Sensor 2 - Fuel-Air Equivalence Ratio", "ratio" } },
    { PID::COMMANDED_EGR, { "Commanded EGR", "%" } },
    { PID::EGR_ERROR, { "EGR Error", "%" } },
    { PID::FUEL_TANK_LEVEL_INPUT, { "Fuel Tank Level Input", "%" } },
    { PID::ABSOLULTE_BAROMETRIC_PRESSURE, { "Absolute Barometric Pressure", "kPa" } },
    { PID::CATALYST_TEMPERATURE_BANK_1_SENSOR_1, { "Catalyst Temperature: Bank 1, Sensor 1", "°C" } },
    { PID::CATALYST_TEMPERATURE_BANK_2_SENSOR_1, { "Catalyst Temperature: Bank 2, Sensor 1", "°C" } },
    { PID::MONITOR_STATUS_THIS_DRIVE_CYCLE, { "Monitor status this drive cycle", "" } },
    { PID::CONTROL_MODULE_VOLTAGE, { "Control module voltage", "V" } },
    { PID::RELATIVE_THROTTLE_POSITION, { "Relative throttle position", "%" } },
    { PID::AMBIENT_AIR_TEMPERATURE, { "Ambient air temperature", "°C" } },
    { PID::FUEL_TYPE, { "Fuel Type", "" } },
    { PID::ETHANOL_FUEL_PERCENTAGE, { "Ethanol fuel percentage", "%" } },
    { PID::ENGINE_OIL_TEMPERATURE, { "Engine oil temperature", "°C" } },
    { PID::FUEL_INJECTION_TIMING, { "Fuel injection timing", "°" } },
    { PID::ENGINE_FUEL_RATE, { "Engine fuel rate", "L/h" } },
    { PID::EMISSION_REQUIREMENT_TO_WHICH_VEHICLE_IS_DESIGNED, { "Emission requirements to which vehicle is designed", "" } },
};


bool OBDReader::isValidPID(PID pid)
{
    return pidMap.find(pid) != pidMap.end();
}

OBDReader::OBDReader(CANManager* can, int time) : canManager(can), timeout(time) {};

OBDReader::~OBDReader() {};




string OBDReader::getPIDName(PID pid) const {
    auto it = pidMap.find(pid);             //auto se refere automaticamente ao tipo de variavel
                                            //it ponteiro para cada elemento do mapa
    if (it != pidMap.end())                // 
        return it->second.name;             //second se refere a value
                                            //first se refere a 
    else
        return "PID desconhecido.";
}

string OBDReader::getPIDName(int pidValue) const
{
    auto it = pidMap.find(static_cast<PID>(pidValue));
    if (it != pidMap.end())
        return it->second.name;
    else
        return "PID desconhecido.";
}

string OBDReader::getPIDUnit(PID pid) const {
    auto it = pidMap.find(pid);
    if (it != pidMap.end())
        return it->second.unit;
    else 
        return "PID desconhecido.";
 
}

string OBDReader::getPIDUnit(int pidValue) const {
    auto it = pidMap.find(static_cast<PID>(pidValue));
    if (it != pidMap.end())
        return it->second.unit;
    else
        return "PID desconhecido.";

}

float OBDReader::pidRead(PID pid)
{
    if (isValidPID(pid))
        return 0;

    //definicao de macros para acelerar leitura da ECU
    uint8_t value[4] = { 0 };   // array de 4 elementos do tipo uint8_t (cada elemento com 8 bits ou 1 byte)
    #define A value[0]          //  primeiro byte
    #define B value[1]          //  segundo byte
    #define C value[2]          //  terceiro byte
    #define D value[3]          //  quarto byte

    switch (pid)
    {
    default:
    case PIDS_SUPPORT_01_20: // raw
    case MONITOR_STATUS_SINCE_DTCS_CLEARED: // raw
    case FREEZE_DTC: // raw
    case PIDS_SUPPORT_21_40: // raw
    case PIDS_SUPPORT_41_60: // raw
    case MONITOR_STATUS_THIS_DRIVE_CYCLE: // raw
        return ((uint32_t)A << 24 | (uint32_t)B << 16 | (uint32_t)C << 8 | (uint32_t)D);   

    case FUEL_SYSTEM_STATUS: // raw
    case RUN_TIME_SINCE_ENGINE_START:
    case DISTANCE_TRAVELED_WITH_MIL_ON:
    case DISTANCE_TRAVELED_SINCE_CODES_CLEARED:
    case TIME_RUN_WITH_MIL_ON:
    case TIME_SINCE_TROUBLE_CODES_CLEARED:
        return (A * 256.0 + B);

    case CALCULATED_ENGINE_LOAD:
    case THROTTLE_POSITION:
    case COMMANDED_EGR:
    case COMMANDED_EVAPORATIVE_PURGE:
    case FUEL_TANK_LEVEL_INPUT:
    case RELATIVE_THROTTLE_POSITION:
    case ABSOLUTE_THROTTLE_POSITION_B:
    case ABSOLUTE_THROTTLE_POSITION_C:
    case ABSOLUTE_THROTTLE_POSITION_D:
    case ABSOLUTE_THROTTLE_POSITION_E:
    case ABSOLUTE_THROTTLE_POSITION_F:
    case COMMANDED_THROTTLE_ACTUATOR:
    case ETHANOL_FUEL_PERCENTAGE:
    case RELATIVE_ACCELERATOR_PEDAL_POSITTION:
    case HYBRID_BATTERY_PACK_REMAINING_LIFE:
        return (A / 2.55);

    case COMMANDED_SECONDARY_AIR_STATUS: // raw
    case OBD_STANDARDS_THIS_VEHICLE_CONFORMS_TO: // raw
    case OXYGEN_SENSORS_PRESENT_IN_2_BANKS: // raw
    case OXYGEN_SENSORS_PRESENT_IN_4_BANKS: // raw
    case AUXILIARY_INPUT_STATUS: // raw
    case FUEL_TYPE: // raw
    case EMISSION_REQUIREMENT_TO_WHICH_VEHICLE_IS_DESIGNED: // raw
        return (A);

    case OXYGEN_SENSOR_1_SHORT_TERM_FUEL_TRIM:
    case OXYGEN_SENSOR_2_SHORT_TERM_FUEL_TRIM:
    case OXYGEN_SENSOR_3_SHORT_TERM_FUEL_TRIM:
    case OXYGEN_SENSOR_4_SHORT_TERM_FUEL_TRIM:
    case OXYGEN_SENSOR_5_SHORT_TERM_FUEL_TRIM:
    case OXYGEN_SENSOR_6_SHORT_TERM_FUEL_TRIM:
    case OXYGEN_SENSOR_7_SHORT_TERM_FUEL_TRIM:
    case OXYGEN_SENSOR_8_SHORT_TERM_FUEL_TRIM:
        return ((B / 1.28) - 100.0);
        break;

    case ENGINE_COOLANT_TEMPERATURE:
    case AIR_INTAKE_TEMPERATURE:
    case AMBIENT_AIR_TEMPERATURE:
    case ENGINE_OIL_TEMPERATURE:
        return (A - 40.0);

    case SHORT_TERM_FUEL_TRIM_BANK_1:
    case LONG_TERM_FUEL_TRIM_BANK_1:
    case SHORT_TERM_FUEL_TRIM_BANK_2:
    case LONG_TERM_FUEL_TRIM_BANK_2:
    case EGR_ERROR:
        return ((A / 1.28) - 100.0);

    case FUEL_PRESSURE:
        return (A * 3.0);

    case INTAKE_MANIFOLD_ABSOLUTE_PRESSURE:
    case VEHICLE_SPEED:
    case WARM_UPS_SINCE_CODES_CLEARED:
    case ABSOLULTE_BAROMETRIC_PRESSURE:
        return (A);

    case ENGINE_RPM:
        return ((A * 256.0 + B) / 4.0);

    case TIMING_ADVANCE:
        return ((A / 2.0) - 64.0);

    case MAF_AIR_FLOW_RATE:
        return ((A * 256.0 + B) / 100.0);

    case FUEL_RAIL_PRESSURE:
        return ((A * 256.0 + B) * 0.079);

    case FUEL_RAIL_GAUGE_PRESSURE:
    case FUEL_RAIL_ABSOLUTE_PRESSURE:
        return ((A * 256.0 + B) * 10.0);

    case OXYGEN_SENSOR_1_FUEL_AIR_EQUIVALENCE_RATIO:
    case OXYGEN_SENSOR_2_FUEL_AIR_EQUIVALENCE_RATIO:
    case OXYGEN_SENSOR_3_FUEL_AIR_EQUIVALENCE_RATIO:
    case OXYGEN_SENSOR_4_FUEL_AIR_EQUIVALENCE_RATIO:
    case OXYGEN_SENSOR_5_FUEL_AIR_EQUIVALENCE_RATIO:
    case OXYGEN_SENSOR_6_FUEL_AIR_EQUIVALENCE_RATIO:
    case OXYGEN_SENSOR_7_FUEL_AIR_EQUIVALENCE_RATIO:
    case OXYGEN_SENSOR_8_FUEL_AIR_EQUIVALENCE_RATIO:
    case 0x34:
    case 0x35:
    case 0x36:
    case 0x37:
    case 0x38:
    case 0x39:
    case 0x3a:
    case 0x3b:
        return (((A * 256.0 + B) * 2.0) / 65536.0);

    case EVAP_SYSTEM_VAPOR_PRESSURE:
        return (((int16_t)(A * 256.0 + B)) / 4.0);

    case CATALYST_TEMPERATURE_BANK_1_SENSOR_1:
    case CATALYST_TEMPERATURE_BANK_2_SENSOR_1:
    case CATALYST_TEMPERATURE_BANK_1_SENSOR_2:
    case CATALYST_TEMPERATURE_BANK_2_SENSOR_2:
        return (((A * 256.0 + B) / 10.0) - 40.0);

    case CONTROL_MODULE_VOLTAGE:
        return ((A * 256.0 + B) / 1000.0);

    case ABSOLUTE_LOAD_VALUE:
        return ((A * 256.0 + B) / 2.55);

    case FUEL_AIR_COMMANDED_EQUIVALENCE_RATE:
        return (2.0 * (A * 256.0 + B) / 65536.0);

    case ABSOLUTE_EVAP_SYSTEM_VAPOR_PRESSURE:
        return ((A * 256.0 + B) / 200.0);

    case 0x54:
        return ((A * 256.0 + B) - 32767.0);

    case FUEL_INJECTION_TIMING:
        return (((A * 256.0 + B) / 128.0) - 210.0);

    case ENGINE_FUEL_RATE:
        return ((A * 256.0 + B) / 20.0);
    }

}

void OBDReader::begin(CANManager* can, int time) {
    can->readCANMessages(time);
}

string OBDReader::vinRead(CANManager* can, int time)
{
    /*
    Exemplo de Payload VCAN:
        vcan0  7DF   [8]  07 09 02 00 00 00 00 00
    
    07: O comprimento dos dados uteis.
    09: Modo de leitura de dados (solicitação de informacoes veiculares).
    02: PID solicitado (VIN do veiculo).
    00 00 00 00 00: Preenchimento ou espaco reservado.
    
    */ 
    char vin[18] = { 0 };                                       // array de 4 elementos do tipo uint8_t (cada elemento com 8 bits ou 1 byte)


    can->sendCANMessage(0x09, 0x02, vin, sizeof(vin));

    return vin;
}



