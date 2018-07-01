import numpy as np
from  policy_network import Network
import pdb
import tensorflow as tf
import random

class Agent_PG():
	def __init__(self, env, load):
		self.env = env
		self.batch_size_episodes=1
		self.checkpoint_every_n_episodes=10
		self.discount_factor=0.9

		self.network = Network()

		if load:
			print('loading trained model')
			self.network.load_checkpoint()

	def train(self,cases):

		batch_state_action_reward_tuples = []
		random_choice_prob = 0
		for episode_n in range(100000):

			print("\n\nStarting episode %d" % episode_n)
			for case in cases:
				observation = self.env.reset(*case)
				state_action_reward_tuples = []
				case_done = False
				move_n = 0
				while (not case_done) and (move_n <100000):
					observation = self.preprocess(observation)
					if random.random() < random_choice_prob :
						action = np.random.choice((0,1,2))
					else :
						action = self.make_action(observation)
					print("move: %d"%move_n,end='\r',flush=True)
					new_observation, reward, case_done= self.env.step(action)
					tup = (observation, action, reward)
					state_action_reward_tuples.append(tup)

					observation = new_observation
					move_n += 1
				print("case %s finished after %d moves, cost = %f, prob = %f" % (case[0].split('/')[2], move_n,self.env.best_origin,random_choice_prob),flush=True)

				states, actions, rewards = zip(*state_action_reward_tuples)
				#rewards = self.discount_rewards(rewards, self.discount_factor)
				#rewards -= np.mean(rewards)
				#rewards /= np.std(rewards)
				state_action_reward_tuples = list(zip(states, actions, rewards))
				batch_state_action_reward_tuples+=state_action_reward_tuples
			
			self.network.train(batch_state_action_reward_tuples)
			batch_state_action_reward_tuples = []

			if episode_n%1 ==0 :
				self.network.save_checkpoint()


			if random_choice_prob > 0 :
				random_choice_prob -= 0.05
	


	def make_action(self, observation):

		act_probability = self.network.choose_action(observation)
		#new_p = act_probability.ravel()/2 + 1/6
		action = np.random.choice( 3, p=act_probability.ravel())
		#action = np.random.choice( 3, p=new_p)
		return action

	def preprocess(self,I):
		oneHot = np.zeros((1,10))
		oneHot[0,int(10*I[0])]+=1
		history = []
		for x in I[1]:
			if x==0:
				history +=[1,0,0]
			elif x==1:
				history +=[0,1,0]
			elif x==2:
				history +=[0,0,1]
		history = np.matrix(history)

		
		return np.hstack((oneHot,history))

	def discount_rewards(self,rewards, discount_factor):
		discounted_rewards = np.zeros_like(rewards)
		for t in range(len(rewards)):
			discounted_reward_sum = 0
			discount = 1
			for k in range(t, len(rewards)):
				discounted_reward_sum += rewards[k] * discount
				discount *= discount_factor
			discounted_rewards[t] = discounted_reward_sum
		return discounted_rewards

	def test(self,cases):

		self.network.load_checkpoint()
		
		cost = []
		moves = []
		case_history = []
		for case in cases:
			observation = self.env.reset(*case)
			state_action_reward_tuples = []
			case_done = False
			move_n = 0
			cost_history=[]
			while not case_done:
				observation =  self.preprocess(observation)
				action = self.make_action(observation)
				print("move: %d  %d"%(move_n,self.env.best_origin),end='\r',flush=True)

				observation, reward, case_done= self.env.step(action)
				cost_history.append(self.env.best_origin)	
				move_n+=1
			print("case %s finished after %d moves, cost = %f" % (case[0].split('/')[2], move_n,self.env.best_origin),flush=True)
			moves.append(move_n)
			case_history.append(cost_history)
		
		return moves,cost,case_history

		
	def rand(self,cases):
		cost = []
		moves = []
		case_history = []
		for case in cases:
			self.env.reset(*case)
			case_done = False
			move_n = 0
			cost_history=[]
			while not case_done:
				action = np.random.choice((0,1,2))
				o,r,case_done = self.env.step(action)
				print("move: %d"%move_n,end='\r',flush=True)
				cost_history.append(self.env.best_origin)	
				move_n+=1
			print("case %s finished after %d moves, cost = %f" % (case[0].split('/')[2], move_n,self.env.best_origin),flush=True)
			moves.append(move_n)
			case_history.append(cost_history)

		return moves,cost,case_history
		
