import threading
import uuid
from typing import Dict, List

from fastapi import HTTPException

from app.schemas.task_request import TaskRequest
from app.schemas.task_response import (
    TaskDetailResponse,
    TaskListResponse,
    TaskResponse,
)


class TaskService:
    _tasks: Dict[str, TaskDetailResponse] = {}
    _lock = threading.Lock()

    @classmethod
    def submit_task(cls, request: TaskRequest) -> TaskResponse:
        task_id = str(uuid.uuid4())

        task_detail = TaskDetailResponse(
            task_id=task_id,
            task_name=request.task_name,
            task_type=request.task_type.value,
            delay_seconds=request.delay_seconds,
            interval_seconds=request.interval_seconds,
            priority=request.priority,
            payload=request.payload,
            status="submitted",
        )

        with cls._lock:
            cls._tasks[task_id] = task_detail

        return TaskResponse(
            success=True,
            message=f"Task '{request.task_name}' submitted successfully",
            task_id=task_id,
            task_name=request.task_name,
            task_type=request.task_type.value,
            priority=request.priority,
        )

    @classmethod
    def get_task(cls, task_id: str) -> TaskDetailResponse:
        with cls._lock:
            task = cls._tasks.get(task_id)

        if task is None:
            raise HTTPException(status_code=404, detail=f"Task '{task_id}' not found")

        return task

    @classmethod
    def list_tasks(cls) -> TaskListResponse:
        with cls._lock:
            tasks: List[TaskDetailResponse] = list(cls._tasks.values())

        return TaskListResponse(
            total=len(tasks),
            tasks=tasks,
        )