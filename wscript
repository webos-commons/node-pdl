#!/usr/bin/env python

from os import system, remove
from os.path import abspath, exists, join
from shutil import rmtree

srcdir = abspath(".")
blddir = "build"
VERSION = "0.0.1"

def set_options(opt):
  opt.tool_options("compiler_cxx")
  opt.tool_options("compiler_cc")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("compiler_cc")
  conf.check_tool("node_addon")

def clean(ctx): 
  if exists("build"): rmtree("build")
#  if exists(build_config): remove(build_config)

def build(bld):
  node_pdl = bld.new_task_gen("cxx", "shlib", "node_addon")
  node_pdl.source = "src/node-pdl.cpp"
  node_pdl.name = "node_pdl"
  node_pdl.target = "node-pdl"
  node_pdl.uselib = ["pdl"]
  node_pdl.includes = ["/usr/include", "/usr/include/SDL"]
  node_pdl.linkflags = ['-lpdl']
