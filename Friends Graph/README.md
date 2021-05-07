Given a network of people, their schools, and friendships, generates a graph based off of friendship, similarly to how social media does to recommend friends you may know. Finds the shortest path from person A, to person B (based on user input) navigating through friend chains. Also returns an arraylist of arraylists of every single friendship "island".
Sample graph visualization:<br />	
	 *         (sam,rutgers)---(jane,rutgers)-----(bob,rutgers)   (sergei,rutgers)<br />
	 * 	 *                           |                 |             |<br />
	 * 	 *                           |                 |             |<br />
	 * 	 *                      (kaitlin,rutgers)   (samir)----(aparna,rutgers)<br />
	 * 	 *                           |                            |<br />
	 * 	 *                           |                            |<br />
	 * 	 *   (ming,penn state)----(nick,penn state)----(ricardo,penn state)<br />
	 * 	 *                           |<br />
	 * 	 *                           |<br />
	 * 	 *                      (heather,penn state)<br />
	 * 	 *<br />
	 * 	 *<br />
	 * 	 *                    (michele,cornell)----(rachel)<br />
	 * 	 *                           |<br />
	 * 	 *                           |<br />
	 * 	 *      (rich,ucla)---(tom,ucla)<br />