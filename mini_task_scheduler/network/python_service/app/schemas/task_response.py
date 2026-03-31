from pydantic import BaseModel


class TaskResponse(BaseModel):
    success: bool
    message: str
    task_id: str