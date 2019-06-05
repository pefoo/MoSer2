# MoSer2

The goal of this project is getting into c++ development. 
Besideis that,...:

MoSer2 is a monitoring solution for linux systems.
It employs a plugin infrastructure, that is capable of loading components during startup.
Reports are generated and send as email notifications.

# Getting started
### Required packages
- libsqlite3-dev
- libcurl14-openssl-dev

### Compiler
A c++17 capable compiler is required.
MoSer2 is currently developed using gcc-8.

### Compiling
To compile and run it local
```
mkdir runtime
cd runtime
cmake ..
make
```

To install it to _/usr/local/_
```
sudo make install 
```

The used configuration file depends on the build type. 
The debug type settings expect the plugin libraries to be located right next to the executable. 
The actualy configuration expects them to be installed at _/usr/local/lib/MoSer2_.

**Dont forget to setup your configuration.**

### Coverage
To execute the code coverage the following tools are required:
- gcov (make sure to use a version that matches your compiler)
- lcov  (version >= 1.14 required with gcc-8)
- genhtml

Run
```
mkdir runtime
cd runtime
cmake -DCMAKE_BUILD_TYPE=Coverage ..
make coverage
```
To execute the code coverage (unit tests will be executed).
The results are found in runtime/lcov-html/index.html.

# Overview
MoSer2 is build on top of a plugin framework. 
Each metric that is created during runtime is created using a dynamically loaded plugin. 
A plugin is responsible for data acquisition and data processing. 
The goal is to store the data in a arbitrary data sink and create html document based reports using the stored data at any given time. 
The reports may be send via email eventually.

## Features
- [x] core plugin framework
- [x] plugin samples (cpu usage, load average)
- [x] first data sink (sqlite3)
- [x] basic report creation capabilities
- [x] send report (mail)
- [x] installation
- [ ] scripting interface for report creation

## Plugins
The plugin framework is based on [dlopen](http://man7.org/linux/man-pages/man3/dlopen.3.html) and thus **linux only**.
To load a plugin, dlopen is used to load a dynamic shared object (aka shared library). 
To create a plugin, dlsym is used to locate the memory address of a factory function, defined in the shared libary.
Using this address the factory function may be called. 
Along with the factory function, a function to delete the plugin must be defined. 

A plugin consist of two main components. 
Actually, both of them are loaded as separate plugins. 
To keep things simple, the unions of both parts is considered a plugin.

**TODO** Move this rather lengthy description somewhere else....
The first one is responsible to create the actual data. 
Therefore the plugin framework calls each registered plugin periodically. 
The plugin class has to inherit from `monitoringpluginbase::MonitorPluginBase` and overwrite the virtual function `virtual imonitorplugin::PluginData::data_vector AcquireDataInternal()const = 0;`. 
This method is called by the plugin framework to acquire the data. 
The data is then stored in a circular buffer. 
Each record stored in the buffer is stored in a data sink eventually. 
To make a plugin loadable factory methods are required. 
You may use the macros `CREATE_DETAULT_CONSTRUCTOR_FACTORY` and `CREATE_DEFAULT_DESTRUCTOR_FACTORY` to add default implementations. 
Add these macros in some header, but do not wrap them in a class. 
See _include/monitoringplugins/cpuplugin/cpuplugin.hpp_ for more information.
To calculate what ever metric the plugin should provide, you may register a file to be read by the plugin framework. 
This file will be read two times, with a second delay. 
Use `void RegisterFileToRead(const std::string& file);` from `monitoringpluginbase::MonitorPluginBase` to register the file. 
Once registered, the two snapshots of the file are passed as arguments to `virtual imonitorplugin::PluginData::data_vector AcquireDataInternal(imonitorplugin::InputFileContent&& input_file) const = 0;`. 
When implementing this function, you may use the data.

The second component are one or more data processors. 
These processors are used during report creation. 
A processor consists of a token and the processor function. 
The token is used in the report template, it is replaced by the processor functions output during report creation. 
Each processor functions get the available records for that plugin at report creation time. 
A processor function may calculate high order metric or generate charts (base64 encoded) to be embedded in the report. Again factory functions are required to make the processor collection loadable from the plugin. 
You may use the macros `CREATE_PROCESSOR_CONSTRUCTOR_FACTORY` and `CREATE_PROCESSOR_DESTRUCTOR_FACTORY` or see the implementation of the cpu plugin processor collection at _include/monitoringplugins/cpuplugin/cpupluginprocessors.hpp_.

During report creation, the tokens in the report are replaced using the values from the data processors. 
To avoid unnecessary calculations, each token value is evaluated only if it is actually required.

Each plugin has to be compiled as a shared library. 
To use the basic plugin framework link `MeasurementPluginBase`. 
For some additional help generating the processors, add `DataProcessorHelper`. 
To detect plugins during startup, the settings hold a base bath and a library name filter.

## Data sinks
The persistence service is capable of using any data sink, that implements the `persistenceservice::IDataAdapter` interface. 
At the moment, only a sqlite3 adapter is available. 
The data of each plugin is stored in a separate table. 
The table names equal the plugin names. 
The column names match the key of the data.

## Reporting
The reporting component is provided as separate executable. 
The report creation is based on a template. 
The template contains tokens which get replaced by the data processors output during report creation. 
The template is written mainly in html. 
Eventually, the result of the replacement is send as an email.

###  Built-in Tokens


|Token|Value|
|---|---|
|**Data less:**||
|%%HOSTNAME%%|The host name of the machine.|
|%%KERNEL_VERSION%%|The kernel version.|
|%%SYSTEM_NAME%%|The system name (most likely linx...).|
|%%MACHINE_ARCH%%|The machine architecture.|
|%%DOMAIN_NAME%%|The current domain name.|
|%%DATETIME%%|The current date and time in the format Www Mmm dd hh:mm:ss yyyy.|
|%%UPTIME%%|The update in the format X days X hours x minutes x seconds. Zero values are omitted|
|**Cpu plugin:**||
|%%CPU_TIME_SERIES_DATA%%|A base 64 encoded time series chart of the cpu usage.|
|%%CPU_USAGE_AVERAGE%%|The average cpu usage. A single value.|
|**Memory plugin:**||
|%%MEMORY_TIME_SERIES_DATA%%|A base 64 encoded time series chart of the memory usage.|
|**Load average plugin:**||
|%%LOAD_TIME_SERIES_DATA%%|A base 64 encoded time series chart of the load average.|
|**Disk plugin:**||
|%%DISK_{device}_TIME_SERIES_DATA%%|A base 64 encoded time series chart of the disk usage. The device name must be specified|
|**Network plugin:**||
|%%NETWORK_TIMES_SERIES_DATA%%|A base 64 encoded time series chart of the network usage.|
