#!/usr/bin/env python3

import asyncio
import websockets

def test_url(url, data=""):
    async def inner():
        async with websockets.connect(url) as websocket:
            await websocket.send(data)
    return asyncio.get_event_loop().run_until_complete(inner())

test_url("ws://localhost:9002/")
