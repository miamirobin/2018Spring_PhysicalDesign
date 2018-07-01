import math
import random
import time
from myType import Terminal, Block, Net
from Tree   import Tree

random.seed(0)

class FloorPlaner:
	def __init__(self):
		self.BoundW = 0; self.BoundH = 0;
		self.A_norm = 0; self.W_norm = 0;
		self.alpha  = 0.5
		self.numBlocks = 0
		self.numTerminals = 0
		self.numNets = 0
		self.runtime = 0

		self.BlockArea = 0
		self.ChipX, self.ChipY = (0,0)
		self.illegal = True

		self.BlockDic = {}
		self.TerminalDic = {}

		self.BlockList = []
		self.NetList   = []
		self.tree = None

	def readBlock(self, filename):
		with open(filename) as f:
			contant = f.readlines()
		contant = [line.split() for line in contant]
		for line in contant:
			if len(line) == 0:
				continue
			elif line[0] == 'Outline:':
				self.BoundW = int(line[1])
				self.BoundH = int(line[2])
			elif line[0] == 'NumBlocks:':
				self.numBlocks = int(line[1])
			elif line[0] == 'NumTerminals:':
				self.numTerminals = int(line[1])
			else:
				if line[1] == 'terminal':
					x = int(line[2])
					y = int(line[3])
					self.TerminalDic[line[0]] = Terminal(line[0], x, y)
				else:
					name   = line[0]
					width  = int(line[1])
					height = int(line[2])
					b = Block(name, width, height)
					self.BlockDic[name] = b
					self.BlockList.append(b)
					self.TerminalDic[name] = b.t
					self.BlockArea += width*height
		'''
		print('Outine: '+str(self.BoundW)+' '+str(self.BoundH))
		print('NumBlocks: '+str(self.numBlocks))
		print('NumTerminals: '+str(self.numTerminals))
		for b in self.BlockList:
			print(b.t.name+'\t'+str(b.w)+'\t'+str(b.h))
		for t in self.TerminalDic:
			if t not in self.BlockDic:
				terminal = self.TerminalDic[t]
				print(terminal.name+' terminal\t'+str(terminal.x)+'\t'+str(terminal.y))
		print('')
		'''

	def readNet(self, filename):
		with open(filename) as f:
			for line in f:
				line = line.split()
				if len(line) == 0:
					continue
				elif line[0] == 'NumNets:':
					self.NumNets = int(line[1])
				elif line[0] == 'NetDegree:':
					degree = int(line[1])
					n = Net()
					for i in range(degree):
						name = f.readline().split()[0]
						n.terminalList.append(self.TerminalDic[name])
					self.NetList.append(n)

	def Observation(self):
		deadspace = self.ChipX * self.ChipY / self.BlockArea -1
		WidthRatio  = 1-self.ChipX / self.BoundW
		HeightRatio = 1-self.ChipY / self.BoundH
		#Area = self.ChipX * self.ChipY / self.A_norm
		#Wire = self.totalwirelength()  / self.W_norm
		#return (deadspace, WidthRatio, HeightRatio, Area, Wire)
		return (deadspace, WidthRatio, HeightRatio)


	def writeResult(self, filename):
		with open(filename,'w') as f:
			self.ChipW, self.ChipH = self.tree.pack()
			f.write(str(self.getCost(0))+'\n')
			f.write(str(self.totalwirelength())+'\n')
			f.write(str(self.ChipW*self.ChipH)+'\n')
			f.write(str(self.ChipW)+' '+str(self.ChipH)+'\n')
			f.write(str(self.runtime)+'\n')
			for b in self.BlockList:
				bX = b.x + (b.h if b.r else b.w)
				bY = b.y + (b.w if b.r else b.h)
				f.write(b.t.name+' '+str(b.x)+' '+str(b.y)+' '+str(bX)+' '+str(bY)+'\n')

	def totalwirelength(self):
		wirelen = 0
		for n in self.NetList:
			wirelen += n.wirelength()
		return wirelen

	def init(self, NumMove=100):
		self.tree = Tree(self.BlockList)
		self.A_norm = 0
		self.W_norm = 0
		for i in range(NumMove):
			for p in range(5):
				self.tree.Pertubation()
			(self.ChipX, self.ChipY) = self.tree.pack()
			self.A_norm += self.ChipX*self.ChipX
			self.W_norm += self.totalwirelength()
		self.A_norm = self.A_norm/100.0
		self.W_norm = self.W_norm/100.0

	def getCost(self, Mode=1):  #Mode_0:(area,wirelength) ; Mode_1: normalized+illegal panelty
		(self.ChipX, self.ChipY) = self.tree.pack()
		if self.illegal:
			self.illegal = (self.ChipX > self.BoundW) or (self.ChipY > self.BoundH)
		if Mode == 0:
			return self.alpha*self.ChipX*self.ChipY + (1-self.alpha)*(self.totalwirelength())
		elif Mode == 1:
			cost = self.alpha*self.ChipX*self.ChipY/self.A_norm + (1-self.alpha)*(self.totalwirelength()/self.W_norm)
			cost += (self.ChipX > self.BoundW)*(self.ChipX-self.BoundW) + (self.ChipY > self.BoundH)*(self.ChipY-self.BoundH)
			return cost


	def SA(self):
		start = time.time()
		self.init()
		lastcost = self.getCost()
		bestcost = lastcost
		bestTree = self.tree.copy()
		T = 5000.0; Coolrate = 0.99;
		uphill = 0; heatup = 0;
		while T > 0.0001:
			for m in range(self.numBlocks*2): 
				self.tree.Pertubation()
				newcost = self.getCost(1)
				delta = newcost - lastcost
				if delta < 0:
					lastcost = newcost
					if newcost < bestcost:
						bestcost = newcost
						bestTree = self.tree.copy()
				elif random.random() < math.exp(-10.0*delta/T):
					lastcost = newcost
					if uphill > self.numBlocks:
						uphill += 1
						break
				else:
					self.tree.Reverse()
			if T < 0.1 and not self.illegal:
				T = 5000
			else:
				T = T*Coolrate
			print('             \rT = '+str(T)+'\r',end='',flush=True)
		self.tree = bestTree.copy()
		self.runtime = time.time() - start
		(self.ChipX, self.ChipY) = self.tree.pack()
		print('--- SA() ---------------------------------------')
		print('Bounding Box: '+str(self.ChipX)+','+str(self.ChipY))
		print('cost_norm: '+str(self.getCost())+'\t cost: '+str(self.getCost(0)))


if __name__ == '__main__':
	print('case: sample')					
	fp = FloorPlaner()
	fp.readBlock('sample.block')
	fp.readNet('sample.nets')
	fp.alpha = 0.5
	fp.init()
	fp.SA()

	print('case: xerox')
	fp = FloorPlaner()
	fp.readBlock('xerox.block')
	fp.readNet('xerox.nets')
	fp.alpha = 0.5
	fp.init()
	fp.SA()
	fp.writeResult('xerox.out')

	print('case: hp')
	fp = FloorPlaner()
	fp.readBlock('hp.block')
	fp.readNet('hp.nets')
	fp.alpha = 0.5
	fp.init()
	fp.SA()
	fp.writeResult('hp.out')

	print('case: apte')
	fp = FloorPlaner()
	fp.readBlock('apte.block')
	fp.readNet('apte.nets')
	fp.alpha = 0.5
	fp.init()
	fp.SA()
	fp.writeResult('apte.out')

	print('case: test')
	fp = FloorPlaner()
	fp.readBlock('test.block')
	fp.readNet('test.nets')
	fp.alpha = 0.5
	fp.init()
	fp.SA()
	fp.writeResult('test.out')

	print('case: ami33')
	fp = FloorPlaner()
	fp.readBlock('ami33.block')
	fp.readNet('ami33.nets')
	fp.alpha = 0.5
	fp.init()
	fp.SA()
	fp.writeResult('ami33.out')

	print('case: ami49')
	fp = FloorPlaner()
	fp.readBlock('ami49.block')
	fp.readNet('ami49.nets')
	fp.alpha = 0.5
	fp.init()
	fp.SA()
	fp.writeResult('ami49.out')

