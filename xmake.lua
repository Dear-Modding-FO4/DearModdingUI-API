set_xmakever("3.0.0")

set_project("dearmoddingui-api")
set_languages("c++23")

target("dearmoddingui-api", function()
	set_kind("headeronly")
	set_default(os.scriptdir() == os.projectdir())
	add_includedirs("include", { public = true })
	add_headerfiles("include/(DearModdingUI/**.h)")
end)

target("api-header-checks", function()
	set_kind("binary")
	set_default(os.scriptdir() == os.projectdir())
	set_languages("c11", "c++23")
	add_deps("dearmoddingui-api")
	add_files("Tests/**.c", "Tests/**.cpp")
end)
