from dataclasses import dataclass
from datetime import datetime, date

@dataclass
class News:
    id: str
    category: str
    topic: str
    title_len: int
    body_len: int

@dataclass
class Click:
    user_id: str
    news_id: str
    start_time: datetime
    duration: int
    category: str
    topic: str
    title_len: int
    body_len: int

@dataclass
class Query:
    id: int
    content: str
    start: datetime
    end: datetime


@dataclass
class ClickMax:
    category: str
    topic: str
    total: int
