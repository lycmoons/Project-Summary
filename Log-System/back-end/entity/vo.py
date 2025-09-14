from pydantic import BaseModel
from datetime import datetime

class Condition(BaseModel):
    time: list[datetime]
    category: list[str]
    title_len: list[int]
    body_len: list[int]
    user_id: list[str]

class NewsId(BaseModel):
    id: str

class NewsCategory(BaseModel):
    category: str

class UserId(BaseModel):
    id: str