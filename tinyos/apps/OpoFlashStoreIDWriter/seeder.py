import random

random.seed()

seeds = []
w = open('node_info.data', 'wb')
w.write("ID        SEED\n")

for i in range(100):
	while(1):
		seed = random.randint(1,4294967295)
		if seed not in seeds:
			seeds.append(seed)
			break

for i in range(len(seeds)):
	n_id = str(i+1)
	spacing = " " * (9-len(n_id))
	line = str(i+1) + spacing + str(seeds[i]) + "\n"
	w.write(line)
