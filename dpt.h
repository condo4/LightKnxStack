/*
 * Project:   LightKnxStack
 * Author:    Fabien Proriol
 * Created:   2023.05.10
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef __DPT__
#define __DPT__

#include <stdint.h>
#include <stddef.h>

struct dpt
{
    uint8_t mainGroup;
    uint8_t subGroup;
    uint8_t index;
};

#define DPT(main, sub)                                                                             \
    {                                                                                              \
        .mainGroup = main, .subGroup = sub, .index = 0                                             \
    }

#define DPT_Switch DPT(1, 1)
#define DPT_Bool DPT(1, 2)
#define DPT_Enable DPT(1, 3)
#define DPT_Ramp DPT(1, 4)
#define DPT_Alarm DPT(1, 5)
#define DPT_BinaryValue DPT(1, 6)
#define DPT_Step DPT(1, 7)
#define DPT_UpDown DPT(1, 8)
#define DPT_OpenClose DPT(1, 9)
#define DPT_Start DPT(1, 10)
#define DPT_State DPT(1, 11)
#define DPT_Invert DPT(1, 12)
#define DPT_DimSendStyle DPT(1, 13)
#define DPT_InputSource DPT(1, 14)
#define DPT_Reset DPT(1, 15)
#define DPT_Ack DPT(1, 16)
#define DPT_Trigger DPT(1, 17)
#define DPT_Occupancy DPT(1, 18)
#define DPT_Window_Door DPT(1, 19)
#define DPT_LogicalFunction DPT(1, 21)
#define DPT_Scene_AB DPT(1, 22)
#define DPT_ShutterBlinds_Mode DPT(1, 23)
#define DPT_Heat_Cool DPT(1, 100)
#define DPT_Switch_Control DPT(2, 1)
#define DPT_Bool_Control DPT(2, 2)
#define DPT_Enable_Control DPT(2, 3)
#define DPT_Ramp_Control DPT(2, 4)
#define DPT_Alarm_Control DPT(2, 5)
#define DPT_BinaryValue_Control DPT(2, 6)
#define DPT_Step_Control DPT(2, 7)
#define DPT_Direction1_Control DPT(2, 8)
#define DPT_Direction2_Control DPT(2, 9)
#define DPT_Start_Control DPT(2, 10)
#define DPT_State_Control DPT(2, 11)
#define DPT_Invert_Control DPT(2, 12)
#define DPT_Control_Dimming DPT(3, 7)
#define DPT_Control_Blinds DPT(3, 8)
#define DPT_Char_ASCII DPT(4, 1)
#define DPT_Char_8859_1 DPT(4, 2)
#define DPT_Scaling DPT(5, 1)
#define DPT_Angle DPT(5, 3)
#define DPT_Percent_U8 DPT(5, 4)
#define DPT_DecimalFactor DPT(5, 5)
#define DPT_Tariff DPT(5, 6)
#define DPT_Value_1_Ucount DPT(5, 10)
#define DPT_Percent_V8 DPT(6, 1)
#define DPT_Value_1_Count DPT(6, 10)
#define DPT_Status_Mode3 DPT(6, 20)
#define DPT_Value_2_Ucount DPT(7, 1)
#define DPT_TimePeriodMsec DPT(7, 2)
#define DPT_TimePeriod10MSec DPT(7, 3)
#define DPT_TimePeriod100MSec DPT(7, 4)
#define DPT_TimePeriodSec DPT(7, 5)
#define DPT_TimePeriodMin DPT(7, 6)
#define DPT_TimePeriodHrs DPT(7, 7)
#define DPT_PropDataType DPT(7, 10)
#define DPT_Length_mm DPT(7, 11)
#define DPT_UElCurrentmA DPT(7, 12)
#define DPT_Brightness DPT(7, 13)
#define DPT_Value_2_Count DPT(8, 1)
#define DPT_DeltaTimeMsec DPT(8, 2)
#define DPT_DeltaTime10MSec DPT(8, 3)
#define DPT_DeltaTime100MSec DPT(8, 4)
#define DPT_DeltaTimeSec DPT(8, 5)
#define DPT_DeltaTimeMin DPT(8, 6)
#define DPT_DeltaTimeHrs DPT(8, 7)
#define DPT_Percent_V16 DPT(8, 10)
#define DPT_Rotation_Angle DPT(8, 11)
#define DPT_Value_Temp DPT(9, 1)
#define DPT_Value_Tempd DPT(9, 2)
#define DPT_Value_Tempa DPT(9, 3)
#define DPT_Value_Lux DPT(9, 4)
#define DPT_Value_Wsp DPT(9, 5)
#define DPT_Value_Pres DPT(9, 6)
#define DPT_Value_Humidity DPT(9, 7)
#define DPT_Value_AirQuality DPT(9, 8)
#define DPT_Value_Time1 DPT(9, 10)
#define DPT_Value_Time2 DPT(9, 11)
#define DPT_Value_Volt DPT(9, 20)
#define DPT_Value_Curr DPT(9, 21)
#define DPT_PowerDensity DPT(9, 22)
#define DPT_KelvinPerPercent DPT(9, 23)
#define DPT_Power DPT(9, 24)
#define DPT_Value_Volume_Flow DPT(9, 25)
#define DPT_Rain_Amount DPT(9, 26)
#define DPT_Value_Temp_F DPT(9, 27)
#define DPT_Value_Wsp_kmh DPT(9, 28)
#define DPT_TimeOfDay DPT(10, 1)
#define DPT_Date DPT(11, 1)
#define DPT_Value_4_Ucount DPT(12, 1)
#define DPT_Value_4_Count DPT(13, 1)
#define DPT_FlowRate_m3 DPT(13, 2)
#define DPT_ActiveEnergy DPT(13, 10)
#define DPT_ApparantEnergy DPT(13, 11)
#define DPT_ReactiveEnergy DPT(13, 12)
#define DPT_ActiveEnergy_kWh DPT(13, 13)
#define DPT_ApparantEnergy_kVAh DPT(13, 14)
#define DPT_ReactiveEnergy_kVARh DPT(13, 15)
#define DPT_LongDeltaTimeSec DPT(13, 100)
#define DPT_Value_Acceleration DPT(14, 0)
#define DPT_Value_Acceleration_Angular DPT(14, 1)
#define DPT_Value_Activation_Energy DPT(14, 2)
#define DPT_Value_Activity DPT(14, 3)
#define DPT_Value_Mol DPT(14, 4)
#define DPT_Value_Amplitude DPT(14, 5)
#define DPT_Value_AngleRad DPT(14, 6)
#define DPT_Value_AngleDeg DPT(14, 7)
#define DPT_Value_Angular_Momentum DPT(14, 8)
#define DPT_Value_Angular_Velocity DPT(14, 9)
#define DPT_Value_Area DPT(14, 10)
#define DPT_Value_Capacitance DPT(14, 11)
#define DPT_Value_Charge_DensitySurface DPT(14, 12)
#define DPT_Value_Charge_DensityVolume DPT(14, 13)
#define DPT_Value_Compressibility DPT(14, 14)
#define DPT_Value_Conductance DPT(14, 15)
#define DPT_Value_Electrical_Conductivity DPT(14, 16)
#define DPT_Value_Density DPT(14, 17)
#define DPT_Value_Electric_Charge DPT(14, 18)
#define DPT_Value_Electric_Current DPT(14, 19)
#define DPT_Value_Electric_CurrentDensity DPT(14, 20)
#define DPT_Value_Electric_DipoleMoment DPT(14, 21)
#define DPT_Value_Electric_Displacement DPT(14, 22)
#define DPT_Value_Electric_FieldStrength DPT(14, 23)
#define DPT_Value_Electric_Flux DPT(14, 24)
#define DPT_Value_Electric_FluxDensity DPT(14, 25)
#define DPT_Value_Electric_Polarization DPT(14, 26)
#define DPT_Value_Electric_Potential DPT(14, 27)
#define DPT_Value_Electric_PotentialDifference DPT(14, 28)
#define DPT_Value_ElectromagneticMoment DPT(14, 29)
#define DPT_Value_Electromotive_Force DPT(14, 30)
#define DPT_Value_Energy DPT(14, 31)
#define DPT_Value_Force DPT(14, 32)
#define DPT_Value_Frequency DPT(14, 33)
#define DPT_Value_Angular_Frequency DPT(14, 34)
#define DPT_Value_Heat_Capacity DPT(14, 35)
#define DPT_Value_Heat_FlowRate DPT(14, 36)
#define DPT_Value_Heat_Quantity DPT(14, 37)
#define DPT_Value_Impedance DPT(14, 38)
#define DPT_Value_Length DPT(14, 39)
#define DPT_Value_Light_Quantity DPT(14, 40)
#define DPT_Value_Luminance DPT(14, 41)
#define DPT_Value_Luminous_Flux DPT(14, 42)
#define DPT_Value_Luminous_Intensity DPT(14, 43)
#define DPT_Value_Magnetic_FieldStrength DPT(14, 44)
#define DPT_Value_Magnetic_Flux DPT(14, 45)
#define DPT_Value_Magnetic_FluxDensity DPT(14, 46)
#define DPT_Value_Magnetic_Moment DPT(14, 47)
#define DPT_Value_Magnetic_Polarization DPT(14, 48)
#define DPT_Value_Magnetization DPT(14, 49)
#define DPT_Value_MagnetomotiveForce DPT(14, 50)
#define DPT_Value_Mass DPT(14, 51)
#define DPT_Value_MassFlux DPT(14, 52)
#define DPT_Value_Momentum DPT(14, 53)
#define DPT_Value_Phase_AngleRad DPT(14, 54)
#define DPT_Value_Phase_AngleDeg DPT(14, 55)
#define DPT_Value_Power DPT(14, 56)
#define DPT_Value_Power_Factor DPT(14, 57)
#define DPT_Value_Pressure DPT(14, 58)
#define DPT_Value_Reactance DPT(14, 59)
#define DPT_Value_Resistance DPT(14, 60)
#define DPT_Value_Resistivity DPT(14, 61)
#define DPT_Value_SelfInductance DPT(14, 62)
#define DPT_Value_SolidAngle DPT(14, 63)
#define DPT_Value_Sound_Intensity DPT(14, 64)
#define DPT_Value_Speed DPT(14, 65)
#define DPT_Value_Stress DPT(14, 66)
#define DPT_Value_Surface_Tension DPT(14, 67)
#define DPT_Value_Common_Temperature DPT(14, 68)
#define DPT_Value_Absolute_Temperature DPT(14, 69)
#define DPT_Value_TemperatureDifference DPT(14, 70)
#define DPT_Value_Thermal_Capacity DPT(14, 71)
#define DPT_Value_Thermal_Conductivity DPT(14, 72)
#define DPT_Value_ThermoelectricPower DPT(14, 73)
#define DPT_Value_Time DPT(14, 74)
#define DPT_Value_Torque DPT(14, 75)
#define DPT_Value_Volume DPT(14, 76)
#define DPT_Value_Volume_Flux DPT(14, 77)
#define DPT_Value_Weight DPT(14, 78)
#define DPT_Value_Work DPT(14, 79)
#define DPT_Access_Data DPT(15, 0)
#define DPT_String_ASCII DPT(16, 0)
#define DPT_String_8859_1 DPT(16, 1)
#define DPT_SceneNumber DPT(17, 1)
#define DPT_SceneControl DPT(18, 1)
#define DPT_DateTime DPT(19, 1)
#define DPT_SCLOMode DPT(20, 1)
#define DPT_BuildingMode DPT(20, 2)
#define DPT_OccMode DPT(20, 3)
#define DPT_Priority DPT(20, 4)
#define DPT_LightApplicationMode DPT(20, 5)
#define DPT_ApplicationArea DPT(20, 6)
#define DPT_AlarmClassType DPT(20, 7)
#define DPT_PSUMode DPT(20, 8)
#define DPT_ErrorClass_System DPT(20, 11)
#define DPT_ErrorClass_HVAC DPT(20, 12)
#define DPT_Time_Delay DPT(20, 13)
#define DPT_Beaufort_Wind_Force_Scale DPT(20, 14)
#define DPT_SensorSelect DPT(20, 17)
#define DPT_ActuatorConnectType DPT(20, 20)
#define DPT_FuelType DPT(20, 100)
#define DPT_BurnerType DPT(20, 101)
#define DPT_HVACMode DPT(20, 102)
#define DPT_DHWMode DPT(20, 103)
#define DPT_LoadPriority DPT(20, 104)
#define DPT_HVACContrMode DPT(20, 105)
#define DPT_HVACEmergMode DPT(20, 106)
#define DPT_ChangeoverMode DPT(20, 107)
#define DPT_ValveMode DPT(20, 108)
#define DPT_DamperMode DPT(20, 109)
#define DPT_HeaterMode DPT(20, 110)
#define DPT_FanMode DPT(20, 111)
#define DPT_MasterSlaveMode DPT(20, 112)
#define DPT_StatusRoomSetp DPT(20, 113)
#define DPT_ADAType DPT(20, 120)
#define DPT_BackupMode DPT(20, 121)
#define DPT_StartSynchronization DPT(20, 122)
#define DPT_Behaviour_Lock_Unlock DPT(20, 600)
#define DPT_Behaviour_Bus_Power_Up_Down DPT(20, 601)
#define DPT_DALI_Fade_Time DPT(20, 602)
#define DPT_BlinkingMode DPT(20, 603)
#define DPT_LightControlMode DPT(20, 604)
#define DPT_SwitchPBModel DPT(20, 605)
#define DPT_PBAction DPT(20, 606)
#define DPT_DimmPBModel DPT(20, 607)
#define DPT_SwitchOnMode DPT(20, 608)
#define DPT_LoadTypeSet DPT(20, 609)
#define DPT_LoadTypeDetected DPT(20, 610)
#define DPT_SABExceptBehaviour DPT(20, 801)
#define DPT_SABBehaviour_Lock_Unlock DPT(20, 802)
#define DPT_SSSBMode DPT(20, 803)
#define DPT_BlindsControlMode DPT(20, 804)
#define DPT_CommMode DPT(20, 1000)
#define DPT_AddInfoTypes DPT(20, 1001)
#define DPT_RF_ModeSelect DPT(20, 1002)
#define DPT_RF_FilterSelect DPT(20, 1003)
#define DPT_StatusGen DPT(21, 1)
#define DPT_Device_Control DPT(21, 2)
#define DPT_ForceSign DPT(21, 100)
#define DPT_ForceSignCool DPT(21, 101)
#define DPT_StatusRHC DPT(21, 102)
#define DPT_StatusSDHWC DPT(21, 103)
#define DPT_FuelTypeSet DPT(21, 104)
#define DPT_StatusRCC DPT(21, 105)
#define DPT_StatusAHU DPT(21, 106)
#define DPT_LightActuatorErrorInfo DPT(21, 601)
#define DPT_RF_ModeInfo DPT(21, 1000)
#define DPT_RF_FilterInfo DPT(21, 1001)
#define DPT_Channel_Activation_8 DPT(21, 1010)
#define DPT_StatusDHWC DPT(22, 100)
#define DPT_StatusRHCC DPT(22, 101)
#define DPT_Media DPT(22, 1000)
#define DPT_Channel_Activation_16 DPT(22, 1010)
#define DPT_OnOff_Action DPT(23, 1)
#define DPT_Alarm_Reaction DPT(23, 2)
#define DPT_UpDown_Action DPT(23, 3)
#define DPT_HVAC_PB_Action DPT(23, 102)
#define DPT_VarString_8859_1 DPT(24, 1)
#define DPT_DoubleNibble DPT(25, 1000)
#define DPT_SceneInfo DPT(26, 1)
#define DPT_CombinedInfoOnOff DPT(27, 1)
#define DPT_UTF_8 DPT(28, 1)
#define DPT_ActiveEnergy_V64 DPT(29, 10)
#define DPT_ApparantEnergy_V64 DPT(29, 11)
#define DPT_ReactiveEnergy_V64 DPT(29, 12)
#define DPT_Channel_Activation_24 DPT(30, 1010)
#define DPT_PB_Action_HVAC_Extended DPT(31, 101)
#define DPT_Heat_Cool_Z DPT(200, 100)
#define DPT_BinaryValue_Z DPT(200, 101)
#define DPT_HVACMode_Z DPT(201, 100)
#define DPT_DHWMode_Z DPT(201, 102)
#define DPT_HVACContrMode_Z DPT(201, 104)
#define DPT_EnablH_Cstage_Z DPT(201, 105)
#define DPT_BuildingMode_Z DPT(201, 107)
#define DPT_OccMode_Z DPT(201, 108)
#define DPT_HVACEmergMode_Z DPT(201, 109)
#define DPT_RelValue_Z DPT(202, 1)
#define DPT_UCountValue8_Z DPT(202, 2)
#define DPT_TimePeriodMsec_Z DPT(203, 2)
#define DPT_TimePeriod10Msec_Z DPT(203, 3)
#define DPT_TimePeriod100Msec_Z DPT(203, 4)
#define DPT_TimePeriodSec_Z DPT(203, 5)
#define DPT_TimePeriodMin_Z DPT(203, 6)
#define DPT_TimePeriodHrs_Z DPT(203, 7)
#define DPT_UFlowRateLiter_per_h_Z DPT(203, 11)
#define DPT_UCountValue16_Z DPT(203, 12)
#define DPT_UElCurrent_Z DPT(203, 13)
#define DPT_PowerKW_Z DPT(203, 14)
#define DPT_AtmPressureAbs_Z DPT(203, 15)
#define DPT_PercentU16_Z DPT(203, 17)
#define DPT_HVACAirQual_Z DPT(203, 100)
#define DPT_WindSpeed_Z DPT(203, 101)
#define DPT_SunIntensity_Z DPT(203, 102)
#define DPT_HVACAirFlowAbs_Z DPT(203, 104)
#define DPT_RelSignedValue_Z DPT(204, 1)
#define DPT_DeltaTimeMsec_Z DPT(205, 2)
#define DPT_DeltaTime10Msec_Z DPT(205, 3)
#define DPT_DeltaTime100Msec_Z DPT(205, 4)
#define DPT_DeltaTimeSec_Z DPT(205, 5)
#define DPT_DeltaTimeMin_Z DPT(205, 6)
#define DPT_DeltaTimeHrs_Z DPT(205, 7)
#define DPT_Percent_V16_Z DPT(205, 17)
#define DPT_TempHVACAbs_Z DPT(205, 100)
#define DPT_TempHVACRel_Z DPT(205, 101)
#define DPT_HVACAirFlowRel_Z DPT(205, 102)
#define DPT_HVACModeNext DPT(206, 100)
#define DPT_DHWModeNext DPT(206, 102)
#define DPT_OccModeNext DPT(206, 104)
#define DPT_BuildingModeNext DPT(206, 105)
#define DPT_StatusBUC DPT(207, 100)
#define DPT_LockSign DPT(207, 101)
#define DPT_ValueDemBOC DPT(207, 102)
#define DPT_ActPosDemAbs DPT(207, 104)
#define DPT_StatusAct DPT(207, 105)
#define DPT_StatusLightingActuator DPT(207, 600)
#define DPT_StatusHPM DPT(209, 100)
#define DPT_TempRoomDemAbs DPT(209, 101)
#define DPT_StatusCPM DPT(209, 102)
#define DPT_StatusWTC DPT(209, 103)
#define DPT_TempFlowWaterDemAbs DPT(210, 100)
#define DPT_EnergyDemWater DPT(211, 100)
#define DPT_TempRoomSetpSetShift_3 DPT(212, 100)
#define DPT_TempRoomSetpSet_3 DPT(212, 101)
#define DPT_TempRoomSetpSet_4 DPT(213, 100)
#define DPT_TempDHWSetpSet_4 DPT(213, 101)
#define DPT_TempRoomSetpSetShift_4 DPT(213, 102)
#define DPT_PowerFlowWaterDemHPM DPT(214, 100)
#define DPT_PowerFlowWaterDemCPM DPT(214, 101)
#define DPT_StatusBOC DPT(215, 100)
#define DPT_StatusCC DPT(215, 101)
#define DPT_SpecHeatProd DPT(216, 100)
#define DPT_Version DPT(217, 1)
#define DPT_VolumeLiter_Z DPT(218, 1)
#define DPT_FlowRate_m3_per_h_Z DPT(218, 2)
#define DPT_AlarmInfo DPT(219, 1)
#define DPT_TempHVACAbsNext DPT(220, 100)
#define DPT_SerNum DPT(221, 1)
#define DPT_TempRoomSetpSetF16_3 DPT(222, 100)
#define DPT_TempRoomSetpSetShiftF16_3 DPT(222, 101)
#define DPT_EnergyDemAir DPT(223, 100)
#define DPT_TempSupplyAirSetpSet DPT(224, 100)
#define DPT_ScalingSpeed DPT(225, 1)
#define DPT_Scaling_Step_Time DPT(225, 2)
#define DPT_TariffNext DPT(225, 3)
#define DPT_MeteringValue DPT(229, 1)
#define DPT_MBus_Address DPT(230, 1000)
#define DPT_Locale_ASCII DPT(231, 1)
#define DPT_Colour_RGB DPT(232, 600)
#define DPT_LanguageCodeAlpha2_ASCII DPT(234, 1)
#define DPT_RegionCodeAlpha2_ASCII DPT(234, 2)
#define DPT_Tariff_ActiveEnergy DPT(235, 1)
#define DPT_Prioritised_Mode_Control DPT(236, 1)
#define DPT_DALI_Control_Gear_Diagnostic DPT(237, 600)
#define DPT_SceneConfig DPT(238, 1)
#define DPT_DALI_Diagnostics DPT(238, 600)
#define DPT_FlaggedScaling DPT(239, 1)
#define DPT_CombinedPosition DPT(240, 800)
#define DPT_StatusSAB DPT(241, 800)

void uint8_to_payload(uint8_t *payload, size_t payload_length, int index, uint8_t value,
                      uint8_t mask);
void int16_to_payload(uint8_t *payload, size_t payload_length, int index, int16_t value,
                      uint16_t mask);
void float16_to_payload(uint8_t *payload, size_t payload_length, int index, double value,
                        uint16_t mask); // mask = 0xFFFF

uint16_t payload_to_uint16(const uint8_t *payload, int index);
double payload_to_float16(const uint8_t *payload, int index);

#endif
