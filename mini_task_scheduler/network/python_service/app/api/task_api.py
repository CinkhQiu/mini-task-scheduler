from fastapi import APIRouter

from app.schemas.task_request import TaskRequest
from app.schemas.task_response import (
    TaskDetailResponse,
    TaskListResponse,
    TaskResponse,
)
from app.services.task_service import TaskService

router = APIRouter(prefix="/tasks", tags=["tasks"])


@router.get("/ping")
def ping():
    return {"message": "pong"}


@router.post("/", response_model=TaskResponse)
def create_task(request: TaskRequest) -> TaskResponse:
    return TaskService.submit_task(request)


@router.get("/", response_model=TaskListResponse)
def list_tasks() -> TaskListResponse:
    return TaskService.list_tasks()


@router.get("/{task_id}", response_model=TaskDetailResponse)
def get_task(task_id: str) -> TaskDetailResponse:
    return TaskService.get_task(task_id)