import tkinter as tk
from tkinter import ttk
import random
import string
import time

BG = "#121212"
GAP = 5
ANIM_STEPS = 8
ANIM_DELAY = 15

class Loopover:
    def __init__(self, root):
        self.root = root
        self.root.configure(bg=BG)
        self.setup_style()

        # Drag & animation state
        self.start_x = None
        self.start_y = None
        self.start_cell = None
        self.locked_direction = None
        self.dx_accum = 0
        self.dy_accum = 0
        self.animating = False
        self.pending_moves = []
        self.drag_moved = False

        # Track last move being animated
        self.last_move_type = None
        self.last_move_index = None
        self.last_move_direction = None

        self.tile = 50
        self.x_offset = 5
        self.y_offset = 5

        # Game state
        self.move_count = 0
        self.timer_running = False
        self.start_time = None
        self.just_shuffled = False
        self.shuffling = False

        # Options
        self.rows = tk.IntVar(value=5)
        self.cols = tk.IntVar(value=5)
        self.use_color = tk.BooleanVar(value=True)
        self.use_letters = tk.BooleanVar(value=True)

        self.build_ui()
        self.bind_updates()
        self.reset()

    # ---------------- UI Setup ----------------
    def setup_style(self):
        style = ttk.Style()
        style.theme_use("clam")
        style.configure("TButton",
                        background="#1E1E1E", foreground="white",
                        padding=8, borderwidth=0, relief="flat", font=("Segoe UI",10,"bold"))
        style.map("TButton", background=[("active","#333333")])
        style.configure("TCheckbutton", background=BG, foreground="white")
        style.configure("TLabel", background=BG, foreground="white")

    def build_ui(self):
        top = ttk.Frame(self.root)
        top.pack(fill="x", padx=10, pady=15)

        ttk.Label(top, text="Rows").pack(side="left", padx =(10,5))
        ttk.Spinbox(top, from_=2, to=10, textvariable=self.rows, width=3).pack(side="left", padx=5)
        ttk.Label(top, text="Cols").pack(side="left")
        ttk.Spinbox(top, from_=2, to=10, textvariable=self.cols, width=3).pack(side="left", padx=5)
        ttk.Checkbutton(top, text="Color", variable=self.use_color).pack(side="left", padx=10)
        ttk.Checkbutton(top, text="Letters", variable=self.use_letters).pack(side="left")
        ttk.Button(top, text="Shuffle", command=lambda: self.shuffle_knuth(fast=True)).pack(side="right", padx=5)
        ttk.Button(top, text="Solve", command=self.solve).pack(side="right", padx=5)

        self.status_label = ttk.Label(self.root, text="Moves: 0 | Time: 0.000s")
        self.status_label.pack(pady=(5,10))

        self.canvas = tk.Canvas(self.root, bg=BG, highlightthickness=0)
        self.canvas.pack(fill="both", expand=True, pady=(0,50))
        self.canvas.bind("<Configure>", self.on_resize)
        self.canvas.bind("<Button-1>", self.on_press)
        self.canvas.bind("<B1-Motion>", self.on_drag)
        self.canvas.bind("<ButtonRelease-1>", self.on_release)

    def bind_updates(self):
        for var in [self.rows, self.cols, self.use_color, self.use_letters]:
            var.trace_add("write", lambda *args: self.reset())

    # ---------------- Grid Setup ----------------
    def reset(self):
        self.rows_n = self.rows.get()
        self.cols_n = self.cols.get()
        values = list(range(self.rows_n * self.cols_n))
        if self.use_letters.get():
            symbols = list(string.ascii_uppercase)
        else:
            symbols = [str(i+1) for i in values]

        self.grid = []
        self.colors = {}
        k = 0
        for i in range(self.rows_n):
            row = []
            for j in range(self.cols_n):
                val = symbols[k % len(symbols)]
                row.append(val)
                self.colors[val] = self.generate_color(k)
                k += 1
            self.grid.append(row)

        self.move_count = 0
        self.timer_running = False
        self.start_time = None
        self.update_status()
        self.draw()

    def generate_color(self, k):
        if not self.use_color.get():
            return "#2D2D2D"
        r = int(100 + 155 * ((self.cols_n - 1) - (k % self.cols_n)) / max(1,self.cols_n-1))
        g = int(100 + 155 * (k // self.cols_n) / max(1,self.rows_n-1))
        b = 180
        return f"#{r:02x}{g:02x}{b:02x}"

    # ---------------- Drawing ----------------
    def draw(self):
        self.canvas.delete("all")
        w = self.canvas.winfo_width()
        h = self.canvas.winfo_height()
        total_width = w - 20
        total_height = h - 20
        self.tile = min(total_width / self.cols_n, total_height / self.rows_n)
        self.x_offset = (w - self.cols_n*self.tile)/2
        self.y_offset = (h - self.rows_n*self.tile)/2

        for i in range(self.rows_n):
            for j in range(self.cols_n):
                val = self.grid[i][j]
                gap = GAP if not self.use_color.get() else 0
                x1 = self.x_offset + j*self.tile + gap/2
                y1 = self.y_offset + i*self.tile + gap/2
                x2 = x1 + self.tile - gap
                y2 = y1 + self.tile - gap
                self.canvas.create_rectangle(x1,y1,x2,y2,fill=self.colors[val],outline="")
                self.canvas.create_text((x1+x2)/2,(y1+y2)/2,text=val,fill="black" if self.use_color.get() else "white",
                                        font=("Segoe UI", max(31,int(self.tile/4)),"bold"))

    def update_status(self):
        t = 0
        if self.timer_running and self.start_time:
            t = time.time() - self.start_time
        self.status_label.config(text=f"Moves: {self.move_count} | Time: {t:.3f}s")

    def on_resize(self,event):
        self.draw()

    # ---------------- Animation ----------------
    def enqueue_move(self, move_type, index, direction):
        self.pending_moves.append((move_type, index, direction))
        if not self.animating:
            self.animating = True  # set immediately to prevent double moves
            self.perform_next_move()

    def perform_next_move(self):
        if not self.pending_moves:
            self.animating = False
            self.update_status()
            if self.check_solved():
                self.timer_running = False
            return

        move_type, index, direction = self.pending_moves.pop(0)
        # Update last_move_* here so force_finish always works correctly
        self.last_move_type = move_type
        self.last_move_index = index
        self.last_move_direction = direction
        self.animate_move(move_type, index, direction, 0)

    def animate_move(self, move_type,index,direction,step,force_finish=False):
        steps = ANIM_STEPS
        items = self.canvas.find_all()

        if force_finish:
            if move_type=="row":
                if direction==1:
                    self.grid[index] = [self.grid[index][-1]] + self.grid[index][:-1]
                else:
                    self.grid[index] = self.grid[index][1:] + [self.grid[index][0]]
            else:
                col = [self.grid[i][index] for i in range(self.rows_n)]
                if direction==1:
                    col = [col[-1]] + col[:-1]
                else:
                    col = col[1:] + [col[0]]
                for i in range(self.rows_n):
                    self.grid[i][index] = col[i]
            self.draw()
            self.update_status()
            self.perform_next_move()
            return

        dx = dy = 0
        if move_type=="row": dx = (self.tile * direction)/steps
        else: dy = (self.tile * direction)/steps

        for i in range(self.rows_n):
            for j in range(self.cols_n):
                idx = i*self.cols_n + j
                if (move_type=="row" and i==index) or (move_type=="col" and j==index):
                    rect = items[idx*2]
                    text = items[idx*2+1]
                    self.canvas.move(rect,dx,dy)
                    self.canvas.move(text,dx,dy)

        step += 1
        if step < steps:
            self.root.after(ANIM_DELAY, lambda:self.animate_move(move_type,index,direction,step))
        else:
            if move_type=="row":
                if direction==1:
                    self.grid[index] = [self.grid[index][-1]] + self.grid[index][:-1]
                else:
                    self.grid[index] = self.grid[index][1:] + [self.grid[index][0]]
            else:
                col = [self.grid[i][index] for i in range(self.rows_n)]
                if direction==1:
                    col = [col[-1]] + col[:-1]
                else:
                    col = col[1:] + [col[0]]
                for i in range(self.rows_n):
                    self.grid[i][index] = col[i]
            self.draw()
            self.update_status()
            self.perform_next_move()

    # ---------------- Drag & Release ----------------
    def on_press(self, e):
        self.start_x = e.x
        self.start_y = e.y
        self.dx_accum = 0
        self.dy_accum = 0
        i = int((e.y - self.y_offset)//self.tile)
        j = int((e.x - self.x_offset)//self.tile)
        i = max(0, min(self.rows_n-1, i))
        j = max(0, min(self.cols_n-1, j))
        self.start_cell = (i, j)
        self.locked_direction = None
        self.drag_moved = False

    def on_drag(self, e):
        if self.tile <= 0: return

        dx = e.x - self.start_x
        dy = e.y - self.start_y
        self.dx_accum += dx
        self.dy_accum += dy

        # Determine new drag direction
        new_direction = None
        if abs(self.dx_accum) > 10 and abs(self.dx_accum) >= abs(self.dy_accum):
            new_direction = "h"
        elif abs(self.dy_accum) > 10 and abs(self.dy_accum) > abs(self.dx_accum):
            new_direction = "v"

        # Handle direction switch
        if self.locked_direction and new_direction and new_direction != self.locked_direction:
            if self.animating:
                self.animate_move(self.last_move_type, self.last_move_index,
                                  self.last_move_direction, 0, force_finish=True)
            # Reset old direction accumulator
            if self.locked_direction == "h":
                self.dx_accum = 0
            else:
                self.dy_accum = 0
            self.locked_direction = new_direction
            i = int((e.y - self.y_offset)//self.tile)
            j = int((e.x - self.x_offset)//self.tile)
            self.start_cell = (max(0, min(self.rows_n-1,i)), max(0, min(self.cols_n-1,j)))

        elif not self.locked_direction and new_direction:
            self.locked_direction = new_direction

        # Enqueue moves
        if self.locked_direction == "h":
            while abs(self.dx_accum) >= self.tile*2/3:
                dir = 1 if self.dx_accum > 0 else -1
                self.enqueue_move("row", self.start_cell[0], dir)
                self.dx_accum -= dir*self.tile
                self.drag_moved = True
        elif self.locked_direction == "v":
            while abs(self.dy_accum) >= self.tile*2/3:
                dir = 1 if self.dy_accum > 0 else -1
                self.enqueue_move("col", self.start_cell[1], dir)
                self.dy_accum -= dir*self.tile
                self.drag_moved = True

        self.start_x = e.x
        self.start_y = e.y

    def on_release(self, e):
        threshold = self.tile / 2
        # Only enqueue leftover if no move occurred during drag
        if not self.drag_moved:
            if self.locked_direction == "h" and abs(self.dx_accum) >= threshold:
                dir = 1 if self.dx_accum > 0 else -1
                self.enqueue_move("row", self.start_cell[0], dir)
            elif self.locked_direction == "v" and abs(self.dy_accum) >= threshold:
                dir = 1 if self.dy_accum > 0 else -1
                self.enqueue_move("col", self.start_cell[1], dir)

        # Reset
        self.dx_accum = 0
        self.dy_accum = 0
        self.locked_direction = None
        self.drag_moved = False

    # ---------------- Shuffle & Solve ----------------
    def shuffle_knuth(self, fast=False):
        self.shuffling = True
        flat = [self.grid[i][j] for i in range(self.rows_n) for j in range(self.cols_n)]
        n = len(flat)
        parity = 0
        for i in range(n):
            for j in range(i+1,n):
                if flat[i] > flat[j]:
                    parity ^= 1
        swaps = []
        for i in range(n-1,0,-1):
            j = random.randint(0,i)
            if i != j:
                swaps.append((i,j))
        new_flat = flat.copy()
        for i,j in swaps:
            new_flat[i],new_flat[j] = new_flat[j],new_flat[i]
        new_parity = 0
        for i in range(n):
            for j in range(i+1,n):
                if new_flat[i] > new_flat[j]:
                    new_parity ^= 1
        if new_parity != parity and n>=2:
            swaps.append((n-1,n-2))
        delay = 75 if fast else 150
        self.move_count = 0
        self.timer_running = False
        self.start_time = None
        def animate_swap(idx):
            if idx >= len(swaps):
                self.shuffling = False
                self.just_shuffled = True
                return
            i,j = swaps[idx]
            flat[i],flat[j] = flat[j],flat[i]
            for r in range(self.rows_n):
                for c in range(self.cols_n):
                    self.grid[r][c] = flat[r*self.cols_n+c]
            self.draw()
            self.root.after(delay, lambda: animate_swap(idx+1))
        animate_swap(0)

    def solve(self):
        self.rows_n = self.rows.get()
        self.cols_n = self.cols.get()
        values = list(range(self.rows_n * self.cols_n))
        if self.use_letters.get():
            symbols = list(string.ascii_uppercase)
        else:
            symbols = [str(i+1) for i in values]
        k=0
        for i in range(self.rows_n):
            for j in range(self.cols_n):
                self.grid[i][j] = symbols[k % len(symbols)]
                k+=1
        self.draw()
        self.move_count = 0
        self.timer_running = False
        self.update_status()

    def check_solved(self):
        k=0
        for i in range(self.rows_n):
            for j in range(self.cols_n):
                val = self.grid[i][j]
                if self.use_letters.get():
                    expected = string.ascii_uppercase[k % 26]
                else:
                    expected = str(k+1)
                if val != expected:
                    return False
                k+=1
        return True


# ---------------- Run ----------------
root = tk.Tk()
root.geometry("700x700")
root.title("loopover")
root.resizable(False, False)
app = Loopover(root)
root.mainloop()
