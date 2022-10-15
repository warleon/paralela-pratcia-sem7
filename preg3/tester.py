from subprocess import Popen, PIPE

N = 2**6
pr = 8
iterations = 100
program_path = "./a.out"

print("\nUsing N:", N)
print("   processors", "  |\t","average(s)")
print("-------------------------------------")

for pr_i in range(1,pr+1):
  avg = 0
  for i in range(iterations):
    p = Popen([program_path], shell=True, stdout=PIPE, stdin=PIPE)
    p.stdin.write((str(N)+"\n").encode())
    p.stdin.write((str(pr_i)+"\n").encode())
    p.stdin.flush()
    result = p.stdout.readline().strip().decode()
    avg += float(result)
  avg = avg / iterations
  print("\t",pr_i, "\t|  ",f'{avg:.20f}')
