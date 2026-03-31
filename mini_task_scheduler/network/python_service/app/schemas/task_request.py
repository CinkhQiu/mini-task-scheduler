from enum import Enum
from typing import Any, Dict, Optional

from pydantic import BaseModel, Field, model_validator


class TaskType(str, Enum):
    ONCE = "once"
    PERIODIC = "periodic"


class TaskRequest(BaseModel):
    task_name: str = Field(..., min_length=1, description="Task name")
    task_type: TaskType = Field(..., description="Task type: once or periodic")
    delay_seconds: int = Field(0, ge=0, description="Delay before first execution in seconds")
    interval_seconds: Optional[int] = Field(
        None, ge=1, description="Execution interval in seconds for periodic tasks"
    )
    priority: int = Field(0, description="Task priority, larger value means higher priority")
    payload: Dict[str, Any] = Field(default_factory=dict, description="Task payload")

    @model_validator(mode="after")
    def validate_periodic_fields(self):
        if self.task_type == TaskType.PERIODIC and self.interval_seconds is None:
            raise ValueError("interval_seconds is required for periodic tasks")
        if self.task_type == TaskType.ONCE:
            self.interval_seconds = None
        return self