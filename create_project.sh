#!/usr/bin/env bash

set -e

PROJECT_NAME="mini_task_scheduler"

mkdir -p ${PROJECT_NAME}

# root
mkdir -p ${PROJECT_NAME}/{build,bin,scripts,docs,tests,third_party}

# common
mkdir -p ${PROJECT_NAME}/common/{include/common,src}

# scheduler module
mkdir -p ${PROJECT_NAME}/scheduler/include/scheduler
mkdir -p ${PROJECT_NAME}/scheduler/src
mkdir -p ${PROJECT_NAME}/scheduler/examples

# scheduler submodules
mkdir -p ${PROJECT_NAME}/scheduler/include/scheduler/core
mkdir -p ${PROJECT_NAME}/scheduler/include/scheduler/service
mkdir -p ${PROJECT_NAME}/scheduler/include/scheduler/thread_pool
mkdir -p ${PROJECT_NAME}/scheduler/include/scheduler/logger
mkdir -p ${PROJECT_NAME}/scheduler/include/scheduler/task
mkdir -p ${PROJECT_NAME}/scheduler/src/core
mkdir -p ${PROJECT_NAME}/scheduler/src/service
mkdir -p ${PROJECT_NAME}/scheduler/src/thread_pool
mkdir -p ${PROJECT_NAME}/scheduler/src/logger
mkdir -p ${PROJECT_NAME}/scheduler/src/task

# network module
mkdir -p ${PROJECT_NAME}/network

# cpp network
mkdir -p ${PROJECT_NAME}/network/cpp_http_server/include/http
mkdir -p ${PROJECT_NAME}/network/cpp_http_server/src
mkdir -p ${PROJECT_NAME}/network/cpp_http_server/examples

mkdir -p ${PROJECT_NAME}/network/cpp_http_server/include/http/server
mkdir -p ${PROJECT_NAME}/network/cpp_http_server/include/http/handler
mkdir -p ${PROJECT_NAME}/network/cpp_http_server/include/http/router
mkdir -p ${PROJECT_NAME}/network/cpp_http_server/include/http/dto
mkdir -p ${PROJECT_NAME}/network/cpp_http_server/src/server
mkdir -p ${PROJECT_NAME}/network/cpp_http_server/src/handler
mkdir -p ${PROJECT_NAME}/network/cpp_http_server/src/router
mkdir -p ${PROJECT_NAME}/network/cpp_http_server/src/dto

# python network
mkdir -p ${PROJECT_NAME}/network/python_service/{app,tests,scripts}
mkdir -p ${PROJECT_NAME}/network/python_service/app/{api,core,schemas,services}

# tests
mkdir -p ${PROJECT_NAME}/tests/{unit,integration,stress}

# docs
mkdir -p ${PROJECT_NAME}/docs/{architecture,api,notes}

# scripts
touch ${PROJECT_NAME}/scripts/build.sh
touch ${PROJECT_NAME}/scripts/run_cpp_server.sh
touch ${PROJECT_NAME}/scripts/run_python_service.sh

# root files
touch ${PROJECT_NAME}/README.md
touch ${PROJECT_NAME}/CMakeLists.txt
touch ${PROJECT_NAME}/.gitignore

# common files
touch ${PROJECT_NAME}/common/include/common/Types.h
touch ${PROJECT_NAME}/common/include/common/Status.h
touch ${PROJECT_NAME}/common/src/Types.cpp

# scheduler files
touch ${PROJECT_NAME}/scheduler/include/scheduler/core/Scheduler.h
touch ${PROJECT_NAME}/scheduler/include/scheduler/core/SchedulerCore.h
touch ${PROJECT_NAME}/scheduler/include/scheduler/service/SchedulerService.h
touch ${PROJECT_NAME}/scheduler/include/scheduler/thread_pool/ThreadPool.h
touch ${PROJECT_NAME}/scheduler/include/scheduler/logger/AsyncLogger.h
touch ${PROJECT_NAME}/scheduler/include/scheduler/task/Task.h
touch ${PROJECT_NAME}/scheduler/include/scheduler/task/TaskQueue.h
touch ${PROJECT_NAME}/scheduler/include/scheduler/task/TaskExecutor.h

touch ${PROJECT_NAME}/scheduler/src/core/Scheduler.cpp
touch ${PROJECT_NAME}/scheduler/src/core/SchedulerCore.cpp
touch ${PROJECT_NAME}/scheduler/src/service/SchedulerService.cpp
touch ${PROJECT_NAME}/scheduler/src/thread_pool/ThreadPool.cpp
touch ${PROJECT_NAME}/scheduler/src/logger/AsyncLogger.cpp
touch ${PROJECT_NAME}/scheduler/src/task/Task.cpp
touch ${PROJECT_NAME}/scheduler/src/task/TaskQueue.cpp
touch ${PROJECT_NAME}/scheduler/src/task/TaskExecutor.cpp

touch ${PROJECT_NAME}/scheduler/examples/demo_scheduler.cpp

# cpp network files
touch ${PROJECT_NAME}/network/cpp_http_server/include/http/server/HttpServerModule.h
touch ${PROJECT_NAME}/network/cpp_http_server/include/http/handler/TaskApiHandler.h
touch ${PROJECT_NAME}/network/cpp_http_server/include/http/router/Router.h
touch ${PROJECT_NAME}/network/cpp_http_server/include/http/dto/TaskRequest.h
touch ${PROJECT_NAME}/network/cpp_http_server/include/http/dto/TaskResponse.h

touch ${PROJECT_NAME}/network/cpp_http_server/src/server/HttpServerModule.cpp
touch ${PROJECT_NAME}/network/cpp_http_server/src/handler/TaskApiHandler.cpp
touch ${PROJECT_NAME}/network/cpp_http_server/src/router/Router.cpp
touch ${PROJECT_NAME}/network/cpp_http_server/src/dto/TaskRequest.cpp
touch ${PROJECT_NAME}/network/cpp_http_server/src/dto/TaskResponse.cpp

touch ${PROJECT_NAME}/network/cpp_http_server/examples/demo_http_server.cpp

# python files
touch ${PROJECT_NAME}/network/python_service/app/main.py
touch ${PROJECT_NAME}/network/python_service/app/api/task_api.py
touch ${PROJECT_NAME}/network/python_service/app/core/config.py
touch ${PROJECT_NAME}/network/python_service/app/schemas/task_request.py
touch ${PROJECT_NAME}/network/python_service/app/schemas/task_response.py
touch ${PROJECT_NAME}/network/python_service/app/services/task_service.py
touch ${PROJECT_NAME}/network/python_service/requirements.txt
touch ${PROJECT_NAME}/network/python_service/tests/test_task_api.py

# test files
touch ${PROJECT_NAME}/tests/unit/test_scheduler.cpp
touch ${PROJECT_NAME}/tests/unit/test_thread_pool.cpp
touch ${PROJECT_NAME}/tests/integration/test_http_scheduler_integration.cpp
touch ${PROJECT_NAME}/tests/stress/test_high_concurrency.cpp

# docs files
touch ${PROJECT_NAME}/docs/architecture/architecture.md
touch ${PROJECT_NAME}/docs/api/http_api.md
touch ${PROJECT_NAME}/docs/notes/todo.md

echo "Project structure created: ${PROJECT_NAME}"