import os.path
import numpy as np
import tensorflow as tf
import random

np.random.seed(0)
tf.set_random_seed(0)

OBSERVATIONS_SIZE = 40

hidden_layer_size=100
learning_rate=0.5
checkpoints_dir='checkpoints'

class Network:

	def __init__(self):
		
		self.build_network()

		config = tf.ConfigProto()
		config.gpu_options.allow_growth = True

		self.sess = tf.Session(config=config)
		
		self.sess.run(tf.global_variables_initializer())
		
		self.saver = tf.train.Saver()
		self.checkpoint_file = os.path.join(checkpoints_dir, 'policy_network.ckpt')

	def build_network(self):

		self.observations = tf.placeholder(tf.float32,[None, OBSERVATIONS_SIZE])
		self.sampled_actions = tf.placeholder(tf.int64, [None])
		self.advantage = tf.placeholder(tf.float32, [None, 1], name='advantage')
		
		self.dense1 = tf.layers.dense(
			inputs=self.observations,
			units=hidden_layer_size,
			activation=tf.nn.relu,
			#kernel_initializer=tf.contrib.layers.xavier_initializer(),
			kernel_initializer=tf.random_normal_initializer(mean=0, stddev=0.3),
			bias_initializer=tf.constant_initializer(0.1),
			name = 'dense1'
		)

		self.dense2 = tf.layers.dense(
			inputs=self.dense1,
			units=hidden_layer_size,
			activation=tf.nn.relu,
			#kernel_initializer=tf.contrib.layers.xavier_initializer(),
			kernel_initializer=tf.random_normal_initializer(mean=0, stddev=0.3),
			bias_initializer=tf.constant_initializer(0.1),
			name = 'dense2'
		)
		
		self.dense3 = tf.layers.dense(
			inputs=self.dense2,
			units=3,
			activation=None,
			#kernel_initializer=tf.contrib.layers.xavier_initializer(),
			kernel_initializer=tf.random_normal_initializer(mean=0, stddev=0.3),
			bias_initializer=tf.constant_initializer(0.1),
			name = 'dense3'
		)

		self.act_probablilty = tf.nn.softmax(self.dense3, name='act_probability')
		
		neg_log_prob = tf.nn.sparse_softmax_cross_entropy_with_logits(logits=self.dense3, labels=self.sampled_actions)
		self.loss = tf.reduce_mean(neg_log_prob * self.advantage)

		optimizer = tf.train.AdamOptimizer(learning_rate)
		self.train_op = optimizer.minimize(self.loss)

	def load_checkpoint(self):
		print("Loading checkpoint...")
		self.saver.restore(self.sess, self.checkpoint_file)

	def save_checkpoint(self):
		print("Saving checkpoint...")
		self.saver.save(self.sess, self.checkpoint_file)

	def choose_action(self, observations):
		act_probablilty = self.sess.run(
			self.act_probablilty,
			feed_dict={self.observations: observations.reshape([1, -1])})
		return act_probablilty

	def train(self, state_action_reward_tuples):
		print("Training with %d tuples" % len(state_action_reward_tuples))

		#random.shuffle(state_action_reward_tuples)
		for x in range(0,len(state_action_reward_tuples),1000):
			states, actions, rewards = zip(*state_action_reward_tuples[x:x+100])
			states = np.vstack(states)
			actions = np.hstack(actions)
			rewards = np.vstack(rewards)

			feed_dict = {
				self.observations: states,
				self.sampled_actions: actions,
				self.advantage: rewards
			}
			self.sess.run(self.train_op, feed_dict)
