class ODESolver:
    # def __init__

    def integrate(self, func, y0, t_eval, **kwargs):
        start = min(t_eval)
        end = max(t_eval)
