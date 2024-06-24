import threading
from .Definitions import DEFAULT_TIMEOUT_JOIN

# Worker API to handle the communications with the Modules
class Worker(threading.Thread):
    def __init__(self, module):
        threading.Thread.__init__(self)
        self.module = module
        self.running = False
        self.daemon = True
        self.worker = None

    def run(self):
        self.running = True
        self.worker = threading.Thread(target=self.module.start_module)
        self.worker.start()

    def stop(self):
        self.running = False
        self.module.stop_worker()
        self.worker.join(1)


class WorkerManager:
    def __init__(self, modules=[]):
        self.modules = modules
        self.workers = []
        self.running = False

    def add_module(self, module):
        self.modules.append(module)

    def add_worker(self, worker):
        self.workers.append(worker)

    def start(self):
        self.running = True
        for module in self.modules:
            worker = Worker(module)
            worker.run()
            self.workers.append(worker)

    def start_all_workers(self):
        for worker in self.workers:
            worker.start()

    def stop_all_workers(self):
        for worker in self.workers:
            worker.join(DEFAULT_TIMEOUT_JOIN)

    def delete_all_workers(self):
        self.workers = []

    def stop(self):
        self.running = False
        for worker in self.workers:
            worker.stop()

    def join(self):
        for worker in self.workers:
            worker.join(1)

    def get_workers(self):
        return self.workers

    def get_modules(self):
        return self.modules

    def get_running(self):
        return self.running

    def get_worker(self, module):
        for worker in self.workers:
            if worker.module == module:
                return worker
        return None

    def get_module(self, worker):
        for module in self.modules:
            if worker.module == module:
                return module
        return None
