<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="16008000">
	<Property Name="SMProvider.SMVersion" Type="Int">201310</Property>
	<Item Name="My Computer" Type="My Computer">
		<Property Name="IOScan.Faults" Type="Str"></Property>
		<Property Name="IOScan.NetVarPeriod" Type="UInt">100</Property>
		<Property Name="IOScan.NetWatchdogEnabled" Type="Bool">false</Property>
		<Property Name="IOScan.Period" Type="UInt">10000</Property>
		<Property Name="IOScan.PowerupMode" Type="UInt">0</Property>
		<Property Name="IOScan.Priority" Type="UInt">9</Property>
		<Property Name="IOScan.ReportModeConflict" Type="Bool">true</Property>
		<Property Name="IOScan.StartEngineOnDeploy" Type="Bool">false</Property>
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="M1M3SupportViews.lvlib" Type="Library" URL="../Views/M1M3SupportViews.lvlib"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="Clear Errors.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Clear Errors.vi"/>
				<Item Name="compatCalcOffset.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatCalcOffset.vi"/>
				<Item Name="compatFileDialog.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatFileDialog.vi"/>
				<Item Name="compatOpenFileOperation.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatOpenFileOperation.vi"/>
				<Item Name="Error Cluster From Error Code.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Cluster From Error Code.vi"/>
				<Item Name="FindCloseTagByName.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/FindCloseTagByName.vi"/>
				<Item Name="FindElement.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/FindElement.vi"/>
				<Item Name="FindElementStartByName.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/FindElementStartByName.vi"/>
				<Item Name="FindEmptyElement.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/FindEmptyElement.vi"/>
				<Item Name="FindFirstTag.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/FindFirstTag.vi"/>
				<Item Name="FindMatchingCloseTag.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/FindMatchingCloseTag.vi"/>
				<Item Name="Open_Create_Replace File.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/Open_Create_Replace File.vi"/>
				<Item Name="ParseXMLFragments.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/ParseXMLFragments.vi"/>
				<Item Name="Read From XML File(array).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/Read From XML File(array).vi"/>
				<Item Name="Read From XML File(string).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/Read From XML File(string).vi"/>
				<Item Name="Read From XML File.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/Read From XML File.vi"/>
				<Item Name="System Exec.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/system.llb/System Exec.vi"/>
				<Item Name="Write to XML File(array).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/Write to XML File(array).vi"/>
				<Item Name="Write to XML File(string).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/Write to XML File(string).vi"/>
				<Item Name="Write to XML File.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/Write to XML File.vi"/>
			</Item>
			<Item Name="AbortLowerM1M3Command.lvclass" Type="LVClass" URL="../Commands/AbortLowerM1M3Command/AbortLowerM1M3Command.lvclass"/>
			<Item Name="AbortRaiseM1M3Command.lvclass" Type="LVClass" URL="../Commands/AbortRaiseM1M3Command/AbortRaiseM1M3Command.lvclass"/>
			<Item Name="ActiveMaintenanceState.lvclass" Type="LVClass" URL="../States/ActiveMaintenanceState/ActiveMaintenanceState.lvclass"/>
			<Item Name="ApplyAberrationByBendingModesCommand.lvclass" Type="LVClass" URL="../Commands/ApplyAberrationByBendingModesCommand/ApplyAberrationByBendingModesCommand.lvclass"/>
			<Item Name="ApplyAberrationByForcesCommand.lvclass" Type="LVClass" URL="../Commands/ApplyAberrationByForcesCommand/ApplyAberrationByForcesCommand.lvclass"/>
			<Item Name="ApplyAOSCorrectionByBendingModesCommand.lvclass" Type="LVClass" URL="../Commands/ApplyAOSCorrectionByBendingModesCommand/ApplyAOSCorrectionByBendingModesCommand.lvclass"/>
			<Item Name="ApplyAOSCorrectionByForcesCommand.lvclass" Type="LVClass" URL="../Commands/ApplyAOSCorrectionByForcesCommand/ApplyAOSCorrectionByForcesCommand.lvclass"/>
			<Item Name="Boot.lvclass" Type="LVClass" URL="../Boot/Boot.lvclass"/>
			<Item Name="BootCommand.lvclass" Type="LVClass" URL="../Commands/BootCommand/BootCommand.lvclass"/>
			<Item Name="ClearAberrationCommand.lvclass" Type="LVClass" URL="../Commands/ClearAberrationCommand/ClearAberrationCommand.lvclass"/>
			<Item Name="ClearAOSCorrectionCommand.lvclass" Type="LVClass" URL="../Commands/ClearAOSCorrectionCommand/ClearAOSCorrectionCommand.lvclass"/>
			<Item Name="Command.lvclass" Type="LVClass" URL="../Commands/Command/Command.lvclass"/>
			<Item Name="Context.lvclass" Type="LVClass" URL="../Context/Context.lvclass"/>
			<Item Name="Controller.lvclass" Type="LVClass" URL="../Controller/Controller.lvclass"/>
			<Item Name="DisableCommand.lvclass" Type="LVClass" URL="../Commands/DisableCommand/DisableCommand.lvclass"/>
			<Item Name="DisabledState.lvclass" Type="LVClass" URL="../States/DisabledState/DisabledState.lvclass"/>
			<Item Name="EnableCommand.lvclass" Type="LVClass" URL="../Commands/EnableCommand/EnableCommand.lvclass"/>
			<Item Name="EnabledState.lvclass" Type="LVClass" URL="../States/EnabledState/EnabledState.lvclass"/>
			<Item Name="EnterMaintenanceCommand.lvclass" Type="LVClass" URL="../Commands/EnterMaintenanceCommand/EnterMaintenanceCommand.lvclass"/>
			<Item Name="ExitCommand.lvclass" Type="LVClass" URL="../Commands/ExitCommand/ExitCommand.lvclass"/>
			<Item Name="ExitMaintenanceCommand.lvclass" Type="LVClass" URL="../Commands/ExitMaintenanceCommand/ExitMaintenanceCommand.lvclass"/>
			<Item Name="FaultState.lvclass" Type="LVClass" URL="../States/FaultState/FaultState.lvclass"/>
			<Item Name="FPGA.lvclass" Type="LVClass" URL="../Resources/FPGA/FPGA.lvclass"/>
			<Item Name="HealthAndStatusControlFIFO.lvclass" Type="LVClass" URL="../Resources/HealthAndStatusControlFIFO/HealthAndStatusControlFIFO.lvclass"/>
			<Item Name="HealthAndStatusDataFIFO.lvclass" Type="LVClass" URL="../Resources/HealthAndStatusDataFIFO/HealthAndStatusDataFIFO.lvclass"/>
			<Item Name="ILCCommunications.lvclass" Type="LVClass" URL="../ILCCommunications/ILCCommunications.lvclass"/>
			<Item Name="InitialState.lvclass" Type="LVClass" URL="../States/InitialState/InitialState.lvclass"/>
			<Item Name="LowerM1M3Command.lvclass" Type="LVClass" URL="../Commands/LowerM1M3Command/LowerM1M3Command.lvclass"/>
			<Item Name="M1M3Settings.lvclass" Type="LVClass" URL="../Settings/M1M3SupportSettings/M1M3Settings.lvclass"/>
			<Item Name="M1M3SupportCommunicator.lvlib" Type="Library" URL="../Communicator/M1M3SupportCommunicator.lvlib"/>
			<Item Name="M1M3SupportGeneral.lvlib" Type="Library" URL="../DataTypes/General/M1M3SupportGeneral.lvlib"/>
			<Item Name="M1M3SupportSettings.lvlib" Type="Library" URL="../DataTypes/Settings/M1M3SupportSettings.lvlib"/>
			<Item Name="M1M3SupportTopics.lvlib" Type="Library" URL="../DataTypes/Topics/M1M3SupportTopics.lvlib"/>
			<Item Name="MaintenanceState.lvclass" Type="LVClass" URL="../States/MaintenanceState/MaintenanceState.lvclass"/>
			<Item Name="ManipulateM1M3Command.lvclass" Type="LVClass" URL="../Commands/ManipulateM1M3Command/ManipulateM1M3Command.lvclass"/>
			<Item Name="ModbusIRQ.lvclass" Type="LVClass" URL="../Resources/ModbusIRQ/ModbusIRQ.lvclass"/>
			<Item Name="ModbusSubnetATxDataFIFO.lvclass" Type="LVClass" URL="../Resources/ModbusSubnetATxDataFIFO/ModbusSubnetATxDataFIFO.lvclass"/>
			<Item Name="ModbusSubnetBTxDataFIFO.lvclass" Type="LVClass" URL="../Resources/ModbusSubnetBTxDataFIFO/ModbusSubnetBTxDataFIFO.lvclass"/>
			<Item Name="ModbusSubnetCTxDataFIFO.lvclass" Type="LVClass" URL="../Resources/ModbusSubnetCTxDataFIFO/ModbusSubnetCTxDataFIFO.lvclass"/>
			<Item Name="ModbusSubnetDTxDataFIFO.lvclass" Type="LVClass" URL="../Resources/ModbusSubnetDTxDataFIFO/ModbusSubnetDTxDataFIFO.lvclass"/>
			<Item Name="ModbusSubnetETxDataFIFO.lvclass" Type="LVClass" URL="../Resources/ModbusSubnetETxDataFIFO/ModbusSubnetETxDataFIFO.lvclass"/>
			<Item Name="ModbusTriggerControl.lvclass" Type="LVClass" URL="../Resources/ModbusTriggerControl/ModbusTriggerControl.lvclass"/>
			<Item Name="Model.lvclass" Type="LVClass" URL="../Model/Model.lvclass"/>
			<Item Name="MoveHardpointActuatorsCommand.lvclass" Type="LVClass" URL="../Commands/MoveHardpointActuatorsCommand/MoveHardpointActuatorsCommand.lvclass"/>
			<Item Name="NiFpgaLv.dll" Type="Document" URL="NiFpgaLv.dll">
				<Property Name="NI.PreserveRelativePath" Type="Bool">true</Property>
			</Item>
			<Item Name="OfflineState.lvclass" Type="LVClass" URL="../States/OfflineState/OfflineState.lvclass"/>
			<Item Name="OuterLoopClockIRQ.lvclass" Type="LVClass" URL="../Resources/OuterLoopClockIRQ/OuterLoopClockIRQ.lvclass"/>
			<Item Name="ParkedMaintenanceState.lvclass" Type="LVClass" URL="../States/ParkedMaintenanceState/ParkedMaintenanceState.lvclass"/>
			<Item Name="ParkedState.lvclass" Type="LVClass" URL="../States/ParkedState/ParkedState.lvclass"/>
			<Item Name="ProcessSettings.lvclass" Type="LVClass" URL="../Settings/ProcessSetting/ProcessSettings.lvclass"/>
			<Item Name="Publisher.lvclass" Type="LVClass" URL="../Publisher/Publisher.lvclass"/>
			<Item Name="RaiseM1M3Command.lvclass" Type="LVClass" URL="../Commands/RaiseM1M3Command/RaiseM1M3Command.lvclass"/>
			<Item Name="RTFPGA.lvlib" Type="Library" URL="../Common_RT_FPGA/RTFPGA.lvlib"/>
			<Item Name="RTHealthAndStatus.lvlib" Type="Library" URL="../Common_RT_HealthAndStatus/RTHealthAndStatus.lvlib"/>
			<Item Name="RTHPILC.lvlib" Type="Library" URL="../Common_RT_HPILC/RTHPILC.lvlib"/>
			<Item Name="RTILC.lvlib" Type="Library" URL="../Common_RT_ILC/RTILC.lvlib"/>
			<Item Name="RTModbus.lvlib" Type="Library" URL="../Common_RT_Modbus/RTModbus.lvlib"/>
			<Item Name="RTModbusFPGA.lvlib" Type="Library" URL="../Common_RT_Modbus/RTModbusFPGA.lvlib"/>
			<Item Name="RTTimestamp.lvlib" Type="Library" URL="../Common_RT_Timestamp/RTTimestamp.lvlib"/>
			<Item Name="RTUtilities.lvlib" Type="Library" URL="../Common_RT_Utilities/RTUtilities.lvlib"/>
			<Item Name="Setting.lvclass" Type="LVClass" URL="../Settings/Settings/Setting.lvclass"/>
			<Item Name="ShutdownCommand.lvclass" Type="LVClass" URL="../Commands/ShutdownCommand/ShutdownCommand.lvclass"/>
			<Item Name="StandbyCommand.lvclass" Type="LVClass" URL="../Commands/StandbyCommand/StandbyCommand.lvclass"/>
			<Item Name="StandbyState.lvclass" Type="LVClass" URL="../States/StandbyState/StandbyState.lvclass"/>
			<Item Name="StartCommand.lvclass" Type="LVClass" URL="../Commands/StartCommand/StartCommand.lvclass"/>
			<Item Name="State.lvclass" Type="LVClass" URL="../States/State/State.lvclass"/>
			<Item Name="StateFactory.lvclass" Type="LVClass" URL="../States/StateFactory/StateFactory.lvclass"/>
			<Item Name="TestAirCommand.lvclass" Type="LVClass" URL="../Commands/TestAirCommand/TestAirCommand.lvclass"/>
			<Item Name="TestForceActuatorCommand.lvclass" Type="LVClass" URL="../Commands/TestForceActuatorCommand/TestForceActuatorCommand.lvclass"/>
			<Item Name="TestHardpointCommand.lvclass" Type="LVClass" URL="../Commands/TestHardpointCommand/TestHardpointCommand.lvclass"/>
			<Item Name="TimestampControlFIFO.lvclass" Type="LVClass" URL="../Resources/TimestampControlFIFO/TimestampControlFIFO.lvclass"/>
			<Item Name="UpdateCommand.lvclass" Type="LVClass" URL="../Commands/UpdateCommand/UpdateCommand.lvclass"/>
			<Item Name="UserEvents.lvclass" Type="LVClass" URL="../UserEvents/UserEvents.lvclass"/>
		</Item>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="M1M3SupportSoftwareClient" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{5C1D3D71-4964-48A7-8FEA-0C69362EA4BC}</Property>
				<Property Name="App_INI_GUID" Type="Str">{82873D46-B86F-494D-8898-B2EDA55724BB}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{8E54FB6B-06B7-40E3-A9E4-8F5E1953C207}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">M1M3SupportSoftwareClient</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../builds/NI_AB_PROJECTNAME/M1M3SupportSoftwareClient</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{6D8B3BAF-8DE6-4D8A-ADBF-1D528E68B4B8}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">M1M3SupportSoftwareClient.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../builds/NI_AB_PROJECTNAME/M1M3SupportSoftwareClient/M1M3SupportSoftwareClient.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../builds/NI_AB_PROJECTNAME/M1M3SupportSoftwareClient/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Source[0].itemID" Type="Str">{FBC2B658-A55E-40C9-AD54-83341EAAB131}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/M1M3SupportViews.lvlib/MainView.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">M1M3SupportSoftwareClient</Property>
				<Property Name="TgtF_internalName" Type="Str">M1M3SupportSoftwareClient</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2017 </Property>
				<Property Name="TgtF_productName" Type="Str">M1M3SupportSoftwareClient</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{637252D4-2612-4EED-B2BE-CF6E3CFA39E9}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">M1M3SupportSoftwareClient.exe</Property>
			</Item>
		</Item>
	</Item>
</Project>
