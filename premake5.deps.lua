THIRD_PARTY = "%{wks.location}/ThirdParty"
HSFW_PATH   = os.getenv("HSFW_PATH")

OutputDir = {}
OutputDir["__Midpart__"] = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
OutputDir["Binaries"] = "%{wks.location}/Binaries/%{OutputDir.__Midpart__}"
OutputDir["Intermediates"] = "%{wks.location}/Binaries-Int/%{OutputDir.__Midpart__}"

ProjectDir = {}
ProjectDir["HSCore"] = "%{wks.location}/HSCore"
ProjectDir["HSEditor"] = "%{wks.location}/HSEditor"
ProjectDir["HSFramework"] = "%{wks.location}/HSFramework"

IncludeDir = {}
IncludeDir["HSCore"] = "%{ProjectDir.HSCore}/Source"
IncludeDir["HSEditor"] = "%{ProjectDir.HSEditor}/Source"
IncludeDir["HSFramework"] = "%{ProjectDir.HSFramework}/Source"
