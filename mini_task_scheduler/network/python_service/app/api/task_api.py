from fastapi import APIRouter

from app.schemas.task_request import TaskRequest
from app.schemas.task_response import TaskResponse
from app.services.task_service import TaskService

router = APIRouter(prefix="/tasks", tags=["tasks"])

@router.get("/ping")
def ping():
    return {"message": "pong"}

@router.post("", response_model=TaskResponse)
def create_task(request: TaskRequest) -> TaskResponse:
    return TaskService.submit_task(request)