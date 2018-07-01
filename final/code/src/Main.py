import Agent
import random
import Environment 
import sys
import numpy as np

np.random.seed(0)
random.seed(0)

testcase_origin = [
	( "test/origin/ami49.block", "test/origin/ami49.nets"),
	( "test/origin/xerox.block", "test/origin/xerox.nets"),
	( "test/origin/ami33.block", "test/origin/ami33.nets"),
	( "test/origin/apte.block", "test/origin/apte.nets"),
	( "test/origin/hp.block", "test/origin/hp.nets"),
]


#test = [( "test/origin/apte.block", "test/origin/apte.nets")]
#test = [( "test/origin/xerox.block", "test/origin/xerox.nets")]
#test = [( "test/origin/ami33.block", "test/origin/ami33.nets")]
#test = [( "test/origin/hp.block", "test/origin/hp.nets")]
#test = [( "test/origin/ami49.block", "test/origin/ami49.nets")]
#test = [( "test/origin/n10.block" , "test/origin/n10.nets"   )]
test = [("test/origin/n30.block"  , "test/origin/n30.nets"   )]
#test = [("test/origin/n50.block"  , "test/origin/n50.nets"   )]
#test = [("test/origin/n100.block" , "test/origin/n100.nets"  )]
#test = [("test/origin/n200.block" , "test/origin/n200.nets"  )]

testcase_new = [
	( "test/origin/n10.block"  , "test/origin/n10.nets"   ),
	( "test/origin/n30.block"  , "test/origin/n30.nets"   ),
	( "test/origin/n50.block"  , "test/origin/n50.nets"   ),
	( "test/origin/n100.block" , "test/origin/n100.nets"  ),
	( "test/origin/n200.block" , "test/origin/n200.nets"  ),
]

env = Environment.SA()

agent = Agent.Agent_PG(env,load=False)

if sys.argv[1] == "train":
	agent.train(testcase_new)

elif sys.argv[1] == "test":
	average_cost = np.zeros((1,5))
	average_move = np.zeros((1,5))
	for x in range(5):
		print("\n\n%d"%x)
		move,cost,history = agent.test(testcase_new)
		average_cost += np.array(cost)
		average_move += np.array(move)
	average_cost = average_cost/5
	average_move = average_move/5
	print(average_move, average_cost)

elif sys.argv[1] == 'rand':
	average_cost = np.zeros((1,5))
	average_move = np.zeros((1,5))
	for x in range(5):
		print("\n\n%d"%x)
		move,cost,history = agent.rand(testcase_new)
		average_cost += np.array(cost)
		average_move += np.array(move)
	average_cost = average_cost/5
	average_move = average_move/5
	print(average_move, average_cost)

elif sys.argv[1] == 'comp':
	_, _, history1 = agent.test(testcase_new)
	_, _, history2 = agent.rand(testcase_new)
	import matplotlib
	matplotlib.use('Agg')
	import matplotlib.pyplot as plt
	for c in range(5):
		x = list(range(len(history1[c][::200])))
		for y in history1[c]:
			if y!=0:
				yy1 = y
				break
		for y in history2[c]:
			if y!=0:
				yy2 = y
				break
		y1 = [ y if y!=0 else yy1 for y in history1[c]]
		y2 = [ y if y!=0 else yy2 for y in history2[c]]
		plt.plot(x,y1[::200],'ro' ,x,y2[::200],'bo')
		plt.savefig('%d'%c+'.png')
		plt.clf()
		




'''
rand
			   cost 	  move 
	ami49 : 20900282.2   41431.5
	xerox : 10685270.225 16142
	ami33 :   695743.65  30130
	apte  : 24625345.45  93686
	hp    :  4967235.35  77530

test

	ami49 : 20798463      41431.5
	xerox : 10711632.75   17757.5
	ami33 : 701885.625    30130
	apte  : 24334181.95   117918.5
	hp    : 4871160.35    82699.6


'''
