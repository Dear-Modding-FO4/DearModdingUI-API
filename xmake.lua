set_xmakever("3.0.0")

set_project("dearmoddingui-api")
set_languages("c++23")

target("dearmoddingui-api", function()
	set_kind("headeronly")
	set_default(os.scriptdir() == os.projectdir())
	add_includedirs("include", { public = true })
	add_headerfiles("include/(DearModdingUI/**.h)")
end)

