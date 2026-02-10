import time

actual_time = time.time() #time since 1970 in seconds
seconds = f"{actual_time:,.4f}"
scientif_time = f"{actual_time:.2e}"
current_time = time.strftime("%b %d %Y")
print(f"Seconds since January 1, 1970: {seconds} or {scientif_time} in scientific notation")
print(current_time)
