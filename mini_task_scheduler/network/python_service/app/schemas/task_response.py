from typing import Any, Dict, Optional

from pydantic import BaseModel


class TaskResponse(BaseModel):
    success: bool
    message: str
    task_id: str
    task_name: str
    task_type: str
    priority: int


class TaskDetailResponse(BaseModel):
    task_id: str
    task_name: str
    task_type: str
    delay_seconds: int
    interval_seconds: Optional[int]
    priority: int
    payload: Dict[str, Any]
    status: str


class TaskListResponse(BaseModel):
    total: int
    tasks: list[TaskDetailResponse]