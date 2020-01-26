# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
from abc import ABC, abstractmethod
from enum import Enum

class eSerializationFormat(Enum):
    BINARY = 0
    JSON = 1

class SEAction(ABC):
    __slots__ = ["_comment"]

    def __init__(self):
        self._comment = None

    def copy(self, action):
        self.clear()
        self._comment = action._comment

    def clear(self):
        self.invalidate_comment()

    def has_comment(self):
        return self._comment is not None

    def get_comment(self):
        return self._comment

    def set_comment(self, comment: str):
        self._comment = comment

    def invalidate_comment(self):
        self._comment = None

    @abstractmethod
    def is_valid(self):
        pass