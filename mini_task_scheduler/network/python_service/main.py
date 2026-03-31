from fastapi import FastAPI
from app.api.task_api import router as task_router

app = FastAPI(title="Mini Task Scheduler API")

app.include_router(task_router)