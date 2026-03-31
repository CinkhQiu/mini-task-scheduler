import uuid

from app.schemas.task_request import TaskRequest
from app.schemas.task_response import TaskResponse


class TaskService:
    @staticmethod
    def submit_task(request: TaskRequest) -> TaskResponse:
        fake_task_id = str(uuid.uuid4())

        return TaskResponse(
            success=True,
            message=f"Task '{request.task_name}' submitted successfully",
            task_id=fake_task_id,
        )