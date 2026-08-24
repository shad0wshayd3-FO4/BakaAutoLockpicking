-- include subprojects
includes("lib/commonlibf4")

-- set project constants
set_project("BakaAutoLockpicking")
set_version("4.0.0")
set_license("GPL-3.0")
set_languages("c++23")
set_warnings("allextra")

-- add common rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- set configs
set_config("commonlib_ini", true)
set_config("commonlib_xbyak", true)

-- require package dependencies
add_requires("effolkronium-random")

-- define targets
target("BakaAutoLockpicking")
    add_rules("commonlibf4.plugin", {
        name = "BakaAutoLockpicking",
        author = "shad0wshayd3"
    })

    -- bind package dependencies
    add_packages("effolkronium-random")

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")

    -- add extra files
    add_extrafiles(".clang-format")

    -- add install files
    add_installfiles("res/Interface/**.txt",        { prefixdir = "Interface/Translations" })
    add_installfiles("res/MCM/**.*",                { prefixdir = "MCM/Config/BakaAutoLockpicking" })
    add_installfiles("res/BakaAutoLockpicking.esm", { prefixdir = "" })
