import numpy as np

def run_episode(env, agent):

    s = env.reset()
    score = 0
    
    while True:
        a = agent(s, env)
        s, r, done, _ = env.step(a)
        score += r
        if done: break
            
    return score
    

def run_nepisodes(env, agent, n):
    scores = np.empty(n)
    for i in range(n):
        scores[i] = run_episode(env, agent)
    return scores
    
    
