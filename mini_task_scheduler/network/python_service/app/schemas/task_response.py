from pydantic import BaseModel


class TaskResponse(BaseModel):
    success: bool
    message: str
    task_id: str
    task_name: str
    task_type: str
    priority: int