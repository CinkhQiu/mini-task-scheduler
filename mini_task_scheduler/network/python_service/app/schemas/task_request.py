from pydantic import BaseModel, Field


class TaskRequest(BaseModel):
    task_name: str = Field(..., min_length=1, description="Task name")
    delay_seconds: int = Field(..., ge=0, description="Delay before execution in seconds")