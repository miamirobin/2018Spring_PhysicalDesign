import random
import math
from FloorPlaner import FloorPlaner

class SA():
	def __init__(self):
		self.fp = FloorPlaner()
		self.reward = 0
		self.Observation = None
		self.cost = 0
		self.T = 5000
		self.cooldown = 0
		self.move_n = 0
		self.bestcost = 10000
		self.bestTree = None
		self.done = False
		self.history=[0]*10
		self.heatup_n = 0
		self.best_origin = 0

	def reset(self, block_file_name, net_file_name):
		self.fp = FloorPlaner()
		self.reward = 0
		self.Observation = None
		self.cost = 0
		self.T = 5000
		self.cooldown = 0
		self.move_n = 0
		self.bestcost = 10000
		self.bestTree = None
		self.done = False
		self.history=[0]*10
		self.heatup_n = 0
		self.best_origin = 0
		
		self.fp.readBlock(block_file_name)
		self.fp.readNet(net_file_name)
		self.fp.init(100)
		self.cost = self.fp.getCost(1)
		self.coolrate = int(math.log(self.fp.numBlocks)*8)
		self.Observation = self.fp.Observation()
		#return (*self.Observation,self.cooldown/1000,self.move_n%10000//100)
		return ( self.cooldown%1764/1764 , self.history)

	def step(self, move):
		if move == 0:
			self.fp.tree.move1()	# Switch
		elif move == 1:
			self.fp.tree.move2()	# DelnIns
		elif move == 2:
			self.fp.tree.move3()	# Rotate
		self.history = [move] + self.history[:9]

		newcost = self.fp.getCost(1)
		reward = newcost - self.cost

		self.reward = reward
		if reward < 0:	
			self.cost = newcost
			self.Observation = self.fp.Observation()
			if newcost < self.bestcost:
				self.bestcost = newcost
				self.bestTree = self.fp.tree.copy()
		elif random.random() < math.exp(-10*reward/self.T):
			self.cost = newcost
			self.Observation = self.fp.Observation()
		else:
			self.fp.tree.Reverse()
			
		if self.T < 0.1 :
			if self.fp.illegal:
				self.T = 5000
				self.heatup_n += 1
				print('                  heatup %d'%self.heatup_n)
			else:
				self.done = True
				self.fp.tree = self.bestTree
				self.cost = self.fp.getCost(1)
		else:
			if self.move_n%self.coolrate == 0 :
				self.T = self.T * 0.99

		self.move_n+=1

		if not self.fp.illegal:
			origin_cost = self.fp.getCost(0)
			if origin_cost < self.best_origin or self.best_origin==0:
				self.best_origin = origin_cost 
		
		return ( (self.cooldown%1764/1764 ,self.history),
				  self.reward,
				  self.done
				)

		#return ((*self.Observation,self.cooldown/1000,self.move_n%10000//100), -1*self.reward, episode_done)


		


