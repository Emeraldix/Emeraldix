cmake_minimum_required(VERSION 3.16)

project(FatCreater C)

add_executable(FatCreater 
	src/main.c
	src/fat_access.c
	src/fat_cache.c
	src/fat_filelib.c
	src/fat_format.c
	src/fat_misc.c
	src/fat_string.c
	src/fat_table.c
	src/fat_write.c
)