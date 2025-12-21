#!/usr/bin/env lua

os = require("os")

FILES = { arg[0] }
CURRENT_DIRECTORY = string.match(arg[0], "(.*)/")

CONTEXT = {
	srcs = {},
	executables = {},
	tests = {
		unit = {},
		component = {},
	},
}

local function print_table(t, indent)
	indent = indent or 0
	local formatting = string.rep(" ", indent)
	for k, v in pairs(t) do
		if type(v) == "table" then
			print(formatting .. k .. ":")
			print_table(v, indent + 1)
		else
			print(formatting .. k .. ": " .. tostring(v))
		end
	end
end

local function get_path(file)
	return CURRENT_DIRECTORY .. "/" .. file
end

-- Include another lua file
function INCLUDE(what)
	local orig = CURRENT_DIRECTORY
	CURRENT_DIRECTORY = CURRENT_DIRECTORY .. "/" .. what
	local dst = CURRENT_DIRECTORY .. "/" .. "meson.lua"
	table.insert(FILES, dst)
	dofile(dst)
	--require(string.format("%s/meson", CURRENT_DIRECTORY))
	CURRENT_DIRECTORY = orig
end

function SRC(file)
	table.insert(CONTEXT.srcs, get_path(file))
end

function SRCS(files)
	for _, f in ipairs(files) do
		SRC(f)
	end
end

function EXECUTABLE(spec)
	CONTEXT.executables[spec["name"]] = get_path(spec[1])
end

function TEST(spec)
	table.insert(CONTEXT.tests[spec["suite"]], get_path(spec[1]))
end

function TESTS(spec)
	local suite = spec["suite"]
	for _, file in ipairs(spec) do
		TEST({ file, suite = suite })
	end
end

INCLUDE("src")

if arg[1] == "config-files" then
	print(table.concat(FILES, " "))
	os.exit(0)
end

if arg[1] == "sources" then
	print(table.concat(CONTEXT.srcs, " "))
	os.exit(0)
end

if arg[1] == "executables" then
	local lines = {}
	for key, val in pairs(CONTEXT.executables) do
		table.insert(lines, key .. " " .. val)
	end
	print(table.concat(lines, "\n"))
	os.exit(0)
end

if arg[1] == "tests" then
	local lines = {}
	for suite, files in pairs(CONTEXT.tests) do
		for _, f in ipairs(files) do
			table.insert(lines, suite .. " " .. f)
		end
	end
	print(table.concat(lines, "\n"))
	os.exit(0)
end

print("Unknown command " .. arg[1])
os.exit(1)
