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
				<Item Name="System Exec.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/system.llb/System Exec.vi"/>
			</Item>
			<Item Name="Command.lvclass" Type="LVClass" URL="../Commands/Command/Command.lvclass"/>
			<Item Name="Context.lvclass" Type="LVClass" URL="../Context/Context/Context.lvclass"/>
			<Item Name="DisableCommand.lvclass" Type="LVClass" URL="../Commands/DisableCommand/DisableCommand.lvclass"/>
			<Item Name="DisabledState.lvclass" Type="LVClass" URL="../States/DisabledState/DisabledState.lvclass"/>
			<Item Name="EnableCommand.lvclass" Type="LVClass" URL="../Commands/EnableCommand/EnableCommand.lvclass"/>
			<Item Name="EnabledState.lvclass" Type="LVClass" URL="../States/EnabledState/EnabledState.lvclass"/>
			<Item Name="ExitCommand.lvclass" Type="LVClass" URL="../Commands/ExitCommand/ExitCommand.lvclass"/>
			<Item Name="FaultState.lvclass" Type="LVClass" URL="../States/FaultState/FaultState.lvclass"/>
			<Item Name="InitialState.lvclass" Type="LVClass" URL="../States/InitialState/InitialState.lvclass"/>
			<Item Name="M1M3Communicator.lvlib" Type="Library" URL="../Communicator/M1M3Communicator.lvlib"/>
			<Item Name="M1M3SupportTypes.lvlib" Type="Library" URL="../Types/M1M3SupportTypes.lvlib"/>
			<Item Name="Model.lvclass" Type="LVClass" URL="../Model/Model.lvclass"/>
			<Item Name="OfflineState.lvclass" Type="LVClass" URL="../States/OfflineState/OfflineState.lvclass"/>
			<Item Name="RTFPGA.lvlib" Type="Library" URL="../Common_RT_FPGA/RTFPGA.lvlib"/>
			<Item Name="RTHealthAndStatus.lvlib" Type="Library" URL="../Common_RT_HealthAndStatus/RTHealthAndStatus.lvlib"/>
			<Item Name="RTHPILC.lvlib" Type="Library" URL="../Common_RT_HPILC/RTHPILC.lvlib"/>
			<Item Name="RTILC.lvlib" Type="Library" URL="../Common_RT_ILC/RTILC.lvlib"/>
			<Item Name="RTModbus.lvlib" Type="Library" URL="../Common_RT_Modbus/RTModbus.lvlib"/>
			<Item Name="RTUtilities.lvlib" Type="Library" URL="../Common_RT_Utilities/RTUtilities.lvlib"/>
			<Item Name="StandbyCommand.lvclass" Type="LVClass" URL="../Commands/StandbyCommand/StandbyCommand.lvclass"/>
			<Item Name="StandbyState.lvclass" Type="LVClass" URL="../States/StandbyState/StandbyState.lvclass"/>
			<Item Name="StartCommand.lvclass" Type="LVClass" URL="../Commands/StartCommand/StartCommand.lvclass"/>
			<Item Name="State.lvclass" Type="LVClass" URL="../States/State/State.lvclass"/>
			<Item Name="StateFactory.lvclass" Type="LVClass" URL="../States/StateFactory/StateFactory.lvclass"/>
			<Item Name="Telemetry.lvclass" Type="LVClass" URL="../Telemetry/Telemetry.lvclass"/>
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
