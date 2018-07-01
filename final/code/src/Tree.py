
import copy
import random
from myType import Terminal, Block, Net

random.seed(0)

class Node:
	def __init__(self,i,block=None,parent=None):
		self.b = block
		self.parent = parent
		self.left = None
		self.right = None
		self.rotate = False
		self.i = i

	def setX(self, x):
		self.b.r = self.rotate
		self.b.setX(x)

	def setY(self, y):
		self.b.r = self.rotate
		self.b.setY(y)

	def getX(self):
		return self.b.x

	def getY(self):
		return self.b.y

	def getwidth(self):
		return self.b.h if self.rotate else self.b.w

	def getheight(self):
		return self.b.w if self.rotate else self.b.h
	

class contourNode:
	def __init__(self, s, e, h):
		self.s = s
		self.e = e
		self.h = h
	
	def __str__(self):
		return str((self.s, self.e, self.h))
	def __repr__(self):
		return str((self.s, self.e, self.h))
		


class Tree:
	# Below are the methods of the BinarySearchTree class. 
	def __init__(self, BlockList):
		#self.root = Node(-1)
		self.lastDel = None
		self.lastOp = None
		self.NodeList = []
		self.ContourLine = []
		prevNode = None
		self.Xmax = 0
		self.Ymax = 0
		for i,b in enumerate(BlockList):
			n = Node(i,b,prevNode)
			if prevNode == None:
				self.root = n
			else:
				if random.random() < 0.5:
					prevNode.right = n
				else:
					prevNode.right = n
			self.NodeList.append(n)
			prevNode = n
	
	def move1(self): #switch
		select = random.sample(self.NodeList,2)
		self.Swap(select[0],select[1])
		self.lastOp = (self.Swap, select[0], select[1])
	
	def move2(self): #delete & insert
		success = False
		while not success:
			select = random.sample(self.NodeList,2)
			success = self.DelnIns(select[0],select[1])
		self.lastOp = (self.DelnIns, select[0], select[1])

	def move3(self): #rotate
		select = random.sample(self.NodeList,1)
		self.Rotate(select[0])
		self.lastOp = (self.Rotate, select[0], select[0])

	def Pertubation(self):
		select = random.sample(self.NodeList,2)
		Op = random.random()
		if Op > 0.66:
			while not self.DelnIns(select[0], select[1]):
				select = random.sample(self.NodeList,2)
			self.lastOp = (self.DelnIns, select[0], select[1])
		elif Op > 0.33:
			self.Swap(select[0], select[1])
			self.lastOp = (self.Swap, select[0], select[1])
		else:
			self.Rotate(select[0])
			self.lastOp = (self.Rotate, select[0], select[1])

	def Reverse(self):
		if self.lastOp[0] == self.Rotate:
			self.Rotate(self.lastOp[1])
		elif self.lastOp[0] == self.Swap:
			self.Swap(self.lastOp[1], self.lastOp[2])
		else:
			Del = self.lastOp[1]
			DelisLeft = (Del.parent.left == Del)
			if(not Del.left == None) and (not Del.right == None):
				print('Reverse error!!')
				return
			elif not Del.left == None:
				Del.left.parent = Del.parent
				if DelisLeft:
					Del.parent.left = Del.left
				else:
					Del.parent.right = Del.left
			elif not Del.right == None:
				Del.right.parent = Del.parent
				if DelisLeft:
					Del.parent.left = Del.right
				else:
					Del.parent.right = Del.right
			else:
				if DelisLeft:
					Del.parent.left = None
				else:
					Del.parent.right = None
			(Del.parent, Del.left, Del.right, DelisLeft) = self.lastDel
			if not Del.left == None:
				if Del.left.parent.left == Del.left:
					Del.left.parent.left = None
				else:
					Del.left.parent.right = None
				Del.left.parent = Del
			if not Del.right == None:
				if Del.right.parent.left == Del.right:
					Del.right.parent.left = None
				else:
					Del.right.parent.right = None
				Del.right.parent = Del
			if DelisLeft:
				Del.parent.left = Del
			else:
				Del.parent.right = Del
				

	def Swap(self, node1, node2):
		tempBlock = node1.b
		node1.b =  node2.b
		node2.b = tempBlock

		tempRotate = node1.rotate
		node1.rotate = node2.rotate
		node2.rotate = tempRotate

		tempId = node1.i
		node1.i = node2.i
		node2.i = tempId
	
	def DelnIns(self, Del, Ins):
		if Del == Ins or Del == self.root:
			return False
		else:
			# delete node
			DelisLeft = (Del == Del.parent.left)
			templastDel = (Del.parent, Del.left, Del.right, DelisLeft)
			if (not Del.left == None) and (not Del.right == None):
				if (not Del.left.right == None) and (not Del.right.left == None):
					return False
				elif Del.left.right == None:
					Del.left.parent = Del.parent
					if DelisLeft:
						Del.parent.left = Del.left
					else:
						Del.parent.right = Del.left
					Del.right.parent = Del.left
					Del.left.right = Del.right
					Del.right = None
					Del.left = None
				elif Del.right.left == None:
					Del.right.parent = Del.parent
					if DelisLeft:
						Del.parent.left = Del.right
					else:
						Del.parent.right = Del.right
					Del.left.parent = Del.right
					Del.right.left = Del.left
					Del.right = None
					Del.left = None
				else:
					print('DelnIns Error!! This Shouldn\'t happened')
			elif not Del.left == None:
				Del.left.parent = Del.parent
				if DelisLeft:
					Del.parent.left = Del.left
				else:
					Del.parent.right = Del.left
				Del.left = None
			elif not Del.right == None:
				Del.right.parent = Del.parent
				if DelisLeft:
					Del.parent.left = Del.right
				else:
					Del.parent.right = Del.right
				Del.right = None
			else:
				if DelisLeft:
					Del.parent.left = None
				else:
					Del.parent.right = None
			self.lastDel = templastDel

			# insert node
			if random.random() > 0.5:
				if not Ins.left == None:
					Ins.left.parent = Del
					if random.random() > 0.1:
						Del.left = Ins.left
					else:
						Del.right = Ins.left
				Ins.left = Del
			else:
				if not Ins.right == None:
					Ins.right.parent = Del
					if random.random() > 0.9:
						Del.left = Ins.right
					else:
						Del.right = Ins.right
				Ins.right = Del
			Del.parent = Ins
		return True


	def Rotate(self, node):
		node.rotate = not node.rotate

	def copy(self):
		tree_copy = copy.deepcopy(self)
		for i in range(len(self.NodeList)):
			tree_copy.NodeList[i].b = self.NodeList[i].b
		return tree_copy



	def pack(self):
		self.packNode(self.root)
		return self.Xmax, self.Ymax

	def packNode(self, node, c=0):
		if node == None:
			return
		if node == self.root:
			self.ContourLine = []
			node.setX(0); node.setY(0);
			self.ContourLine.append(contourNode(0,node.getwidth(),node.getheight()))
			self.ContourLine.append(contourNode(node.getwidth(),100000000,0))
			self.Xmax, self.Ymax = node.getwidth(), node.getheight()
		elif node == node.parent.left:
			node.setX(node.parent.getX()+node.parent.getwidth())
			if (node.getX()+node.getwidth()) > self.Xmax:
				self.Xmax = node.getX()+node.getwidth()
			c = self.updateContour(node, c)
		elif node == node.parent.right:
			node.setX(node.parent.getX())
			if (node.getX()+node.getwidth()) > self.Xmax:
				self.Xmax = node.getX()+node.getwidth()
			c = self.updateContour(node, c)
		else:
			print('this shouldn\'t happened')
		#print('Packing Node: '+node.b.t.name+'\t'+str(node.getX())+','+str(node.getY()))
		self.packNode(node.left, c+1)
		self.packNode(node.right, c)

	def updateContour(self, node, c=0):
		x = node.getX()
		X = node.getX() + node.getwidth()
		y = 0
		while c < len(self.ContourLine):
			contour = self.ContourLine[c]
			if x >= contour.e:
				c += 1
			elif X <= contour.s:
				break
			else:
				if contour.h > y:
					y = contour.h
				if   contour.s >= x and contour.e <= X:
					self.ContourLine.remove(contour)
				elif contour.s <= x and contour.e <= X:
					contour.e = x
					c += 1
				elif contour.s >= x and contour.e >= X:
					contour.s = X
					break
				else:
					self.ContourLine.insert(c,contourNode(contour.s,x,contour.h))
					c += 1
					contour.s = X
					break
		node.setY(y)
		Y = y + node.getheight()
		if Y > self.Ymax:
			self.Ymax = Y
		self.ContourLine.insert(c, contourNode(x,X,Y))
		return c
		
		
'''
b1 = Block('A',50,40)
b2 = Block('B',50,60)
b3 = Block('C',50,60)
b4 = Block('D',50,40)
B = [b1,b2,b3,b4]

tree = Tree(B)

tree.pack()
print(tree.ContourLine)

for i in range(1000):
	tree.Pertubation()
	tree.pack()
	print(tree.ContourLine)
	#tree.Reverse()
	#tree.pack()
	#print(tree.ContourLine)

tree1 = copy.deepcopy(tree)
'''
