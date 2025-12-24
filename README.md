# Project-2-COMP2020 (For Window)
**HDPE là ngon luôn 😋**

⚠️ **Attention:**
* Clone this repository into the folder that you use for **Lab 6** (in order to use Docker for riscv interpreter).
* You must have **gcc** and **mingw32 or mingw64** to use the "gcc" and "make" code (this instruction use **mingw32**).

---

## 1. Associative Linked List
* Open cmd and go to the directory that you store the repository.
* Run `mingw32-make linkedlist` to create a `linkedlist` executable to run. This will run the code found in `linkedlist_main.c`.
* After running, it will return a file named `linkedlist.exe`.
* Run that file on cmd or vscode and check the output (if corrects, it will output as below)
```bash
Adding mapping from 10 -> 123
Get 10 -> 123 (expected 123)
Adding mapping from 10 -> 256
Get 10 -> 256 (expected 256)
Size = 1 (expected 1)
Adding mapping from 20 -> 9
Get 20 -> 9 (expected 9)
Size = 2 (expected 2)
```

---

## 2. Hash Table
* Open cmd and go to the directory that you store the repository.
* Run  `mingw32-make hashtable` to create a `hashtable` executable to run. This will run the code found in `hashtable_main.c`.
* After running, it will return a file named `hashtable.exe`.
* Run that file on cmd or vscode and check the output (if corrects, it will output as below)
```bash
Adding mapping from 10 -> 123
Get 10 -> 123 (expected 123)
Adding mapping from 10 -> 256
Get 10 -> 256 (expected 256)
Size = 1 (expected 1)
Adding mapping from 20 -> 9
Get 20 -> 9 (expected 9)
Size = 2 (expected 2)
```

---

## 3. RISC-V Interpreter
* Run **Docker** on your computer.
* Open cmd and go to the directory that you store the repository.
* Run this code to start a container
```ini
docker run -it --rm -v %cd%:/work -w /work stanleynguyen7590/riscv32-lab-full
```
* You will now be inside the Linux container (your prompt will change to something like `root@abcdef:/app#`).
* Run
```ini
gcc -o riscv_interpreter riscv_interpreter.c riscv.c hashtable.c linkedlist.c
```
to create a `riscv_interpreter` executable to run. This will run the code found in `riscv_interpreter.c`.
* After running, it will return a file named `riscv_interpreter`.
* Run that file on cmd using `./riscv_interpreter` and then you can test it with some **Assembly Code** to prove it works :)

### ⭐ This RISCV Interpreter has **Debugging and Assigning Starting Value** functions (See details in Project 2's Assigment Instruction).

### **🔔 Merry Christmas 🎄**
