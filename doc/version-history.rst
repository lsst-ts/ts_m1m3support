.. _Version_History:

===============
Version History
===============

v2.15.2
-------

* utils dcelevation, dcrawdump utilities
* Read and store raw DC accelerometer data
* Reject requests to change slew controller settings if slew flags is active

v2.15.1
-------

* Moved DataTypes to cRIOcpp, ILCWarning singleton
* Use cRIO/SAL/Command.h ACK_ constants
* ReloadConfigurationCommand, fix ForceComponent's settings reloading
* Fix DC accelerometer unit - it's in degrees, needs to be feed as radians to acceleration force calculation
* Fix X DC accelerometer angular acceleration sign
* Fixed Makefiles

v2.15.0
-------

* Raising and lowering can be paused and resumed

v2.14.0
-------

* SlewController settings
* Command to chaneg SlewController settings on-fly
* Reorganized booster valve management
* m1m3sscli command to read LVDTs
* pre-commit hooks

v2.13.0
-------

* Set and ClearSlewFlag commands
* Command to enable/disable force components

v2.12.4
-------
* Calculations of mirror accelerations from TMA velocities (changes)
* DC Accelerometer FPGA code improvements

v2.12.3
-------

* Fixed DC Accelerometers calculations

v2.12.2
-------

* Distribute disabled FA forces into their quadrants - allow force balancing to be activated

v2.12.1
-------

* Don't test FA following error in disabled state

v2.12.0
-------

* Configuration is stored in standard LSST configuration format
* Sends proper messages during CSC initialization

v2.11.0
-------

* Don't turn on lights on startup
* Booster valve automatic activation (based on accelerometers/following error)
* Measured forces limits
