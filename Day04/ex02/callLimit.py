def callLimit(limit: int):
    count=0
    def callLimiter(function):
        def limit_function(*args: Any, **kwds: Any):
            try:
                nonlocal count
                count += 1
                if count <= limit:
                    return function(*args, **kwds)
                else:
                    raise AssertionError(f"{function} call too many times")
            except AssertionError as error:
                print("Error:", error)
        return limit_function

    return callLimiter