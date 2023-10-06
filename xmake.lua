-- 设置平台
set_config("plat", os.host())

-- 工程名
set_project("DreamSky")

-- 工程版本
-- set_version("1.0.0", {build = "%Y%m%d%H%M", soname = true})
set_version("1.0.0", {build = "%Y%m%d%H%M"})

-- 设置需要的最低xmake版本
set_xmakever("2.3.1")

-- 设置语言标准
set_languages("cxx11", "c11")
set_warnings("all")     -- -Wall
--set_symbols("debug")    -- dbg symbols
--add_rules("mode.debug", "mode.release")

-- 导出符号需要
add_defines("DREAMSKY_EXPORTS")

-- 包含参与编译的头文件目录
function dream_directory() 
    local res = {}
    for _, dream in ipairs(os.dirs("./*")) do
        local tmp_dir = path.join(dream, "include")
        if (os.exists(tmp_dir)) then
            table.insert(res, dream)
        end
    end
    return res
end

for _, dream in ipairs(dream_directory()) do 
    dream = dream .. "/include"
    add_includedirs(dream)
end

-- 第三方的依赖头文件
-- 需要区分Windows和Linux
-- local headers_3d = {rapidjson = "1.1.0"}
-- local libs_3d = {}
-- local base_3d = "3d_part"
-- local include_3d = "include"
-- local lib_3d = "lib"

-- if is_os("windows") then
--     headers_3d["pthread"] = "3.0.0"
--     libs_3d["pthread"] = "3.0.0"
-- end

-- for name, version in pairs(headers_3d) do
--     version = "v" .. version
--     local dir = path.join(base_3d, name, version, include_3d)
--     add_includedirs(dir)
-- end

-- for name, version in pairs(libs_3d) do 
--     version = "v" .. version
--     local dir = path.join(base_3d, name, version, lib_3d)
--     add_linkdirs(dir)
-- end

add_requires("rapidjson")
if is_os("windows") then 
    add_requires("pthreads4w")
end

-- 安装文件
-- add_installfiles("DreamHeaders/v1.0.0/include/**.h", {prefixdir = "include/DreamSky"})
-- add_installfiles("(**.md)", {prefixdir = "doc"})

-- 包含子项目
for _, dream in ipairs(dream_directory()) do 
    includes(dream)
end
