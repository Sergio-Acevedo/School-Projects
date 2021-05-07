//package friends;

import java.util.ArrayList;

//import structures.Queue;
//import structures.Stack;

public class Friends {

	/**
	 * Finds the shortest chain of people from p1 to p2.
	 * Chain is returned as a sequence of names starting with p1,
	 * and ending with p2. Each pair (n1,n2) of consecutive names in
	 * the returned chain is an edge in the graph.
	 *
	 * @param g Graph for which shortest chain is to be found.
	 * @param p1 Person with whom the chain originates
	 * @param p2 Person at whom the chain terminates
	 * @return The shortest chain from p1 to p2. Null or empty array list if there is no
	 *         path from p1 to p2
	 *
	 *         (sam,rutgers)---(jane,rutgers)-----(bob,rutgers)   (sergei,rutgers)
	 *                           |                 |             |
	 *                           |                 |             |
	 *                      (kaitlin,rutgers)   (samir)----(aparna,rutgers)
	 *                           |                            |
	 *                           |                            |
	 *   (ming,penn state)----(nick,penn state)----(ricardo,penn state)
	 *                           |
	 *                           |
	 *                      (heather,penn state)
	 *
	 *
	 *                    (michele,cornell)----(rachel)
	 *                           |
	 *                           |
	 *      (rich,ucla)---(tom,ucla)
	 */
	public static ArrayList<String> shortestChain(Graph g, String p1, String p2) {
//		System.out.println("From " + p1 + ", to " + p2 + ":\n");
		ArrayList<String> result = new ArrayList<>();
		ArrayList<String> test = new ArrayList<>();
		ArrayList<ArrayList<String>> solutionHistory = new ArrayList<>();
		result.add(p1);
		test.add(p1);
		result = recursion1(g, p1, p2, result, test, solutionHistory);
		if(result.contains(p2)){
			return result;
		}
		return null;
	}

	private static ArrayList<String> recursion1(Graph g, String current, String p2, ArrayList<String> result, ArrayList<String> test, ArrayList<ArrayList<String>> solutionHistory) {
		System.out.println(current);
		if(!current.equals(p2)){
			String firstFriendOfCurrent = g.members[g.members[g.map.get(current)].first.fnum].name;
			if(!test.contains(firstFriendOfCurrent)){
				ArrayList<String> test2 = new ArrayList<>(test);
				test2.add(firstFriendOfCurrent);
				result = recursion1(g, firstFriendOfCurrent, p2, result, test2, solutionHistory );
			}
			Friend node = g.members[g.map.get(current)].first;
			if(node.next != null) {
				//node = node.next;
				while (node != null) {
					String friendOfCurrent = g.members[node.fnum].name;
					if (!test.contains(friendOfCurrent)) {
						ArrayList<String> test3 = new ArrayList<>(test);
						test3.add(friendOfCurrent);
						result = recursion1(g, friendOfCurrent, p2, result, test3, solutionHistory);
						if(friendOfCurrent.equals(p2)){break;}
					}
					node = node.next;
				}
			}
		}
		else{
			if(!solutionHistory.contains(test)) {
				solutionHistory.add(test);
				//System.out.println("Different route(s) found: " + solutionHistory);
			}
			for(ArrayList<String> x : solutionHistory){
				if(x.size() < result.size() || result.size() == 1){
					result = x;
				}
			}
		}
		return result;
	}

	/**
	 * Finds all cliques of students in a given school.
	 *
	 * Returns an array list of array lists - each constituent array list contains
	 * the names of all students in a clique.
	 *
	 * @param g Graph for which cliques are to be found.
	 * @param school Name of school
	 * @return Array list of clique array lists. Null or empty array list if there is no student in the
	 *         given school
	 *
	 *         (sam,rutgers)---(jane,rutgers)-----(bob,rutgers)   (sergei,rutgers)
	 * 	 *                           |                 |             |
	 * 	 *                           |                 |             |
	 * 	 *                      (kaitlin,rutgers)   (samir)----(aparna,rutgers)
	 * 	 *                           |                            |
	 * 	 *                           |                            |
	 * 	 *   (ming,penn state)----(nick,penn state)----(ricardo,penn state)
	 * 	 *                           |
	 * 	 *                           |
	 * 	 *                      (heather,penn state)
	 * 	 *
	 * 	 *
	 * 	 *                    (michele,cornell)----(rachel)
	 * 	 *                           |
	 * 	 *                           |
	 * 	 *      (rich,ucla)---(tom,ucla)
	 */
	public static ArrayList<ArrayList<String>> cliques(Graph g, String school) {
		ArrayList<String> checkedNames = new ArrayList<>();
		ArrayList<ArrayList<String>> result = new ArrayList<>();
		for(int i = 0; i < g.members.length; i++){
			if(g.members[i].student = true) {
				if(g.members[i].school != null) {
					if (g.members[i].school.equals(school) && !checkedNames.contains(g.members[i].name)) {
						ArrayList<String> x = new ArrayList<>();
						x.add(g.members[i].name);
						x.addAll(recursion2(g, i, school, x));
						ArrayList<String> z = new ArrayList<>();
						for(int j = 0; j < x.size()/2; j++){
							z.add(x.get(j));
						}
						result.add(z);
						for (String y : z) {
							if (!checkedNames.contains(y)) {
								checkedNames.add(y);
							}
						}
					}
				}
			}
		}
		if(result.size() != 0){
			return result;
		}
		return null;
	}

	private static ArrayList<String> recursion2(Graph g, int index, String school, ArrayList<String> checkedNames ){
		if(g.members[g.members[index].first.fnum].school != null) {
			String schoolOfFirstFriendOfCurrent = g.members[g.members[index].first.fnum].school;
			if(schoolOfFirstFriendOfCurrent.equals(school) && !checkedNames.contains(g.members[g.members[index].first.fnum].name)){
				checkedNames.add(g.members[g.members[index].first.fnum].name);
				checkedNames = recursion2(g, g.members[index].first.fnum, school, checkedNames);
			}
			Friend node = g.members[index].first;
			if(node.next != null){
				node = node.next;
				while(node != null){
					if(g.members[node.fnum].school != null) {
						if (g.members[node.fnum].school.equals(school) && !checkedNames.contains(g.members[node.fnum].name)) {
							checkedNames.add(g.members[node.fnum].name);
							if (node.next != null) {
								checkedNames = recursion2(g, node.next.fnum, school, checkedNames);
							}
						}
					}
					node = node.next;
				}
			}
		}
		return checkedNames;
	}


	/**
	 * Finds and returns all connectors in the graph.
	 *
	 * @param g Graph for which connectors needs to be found.
	 * @return Names of all connectors. Null or empty array list if there are no connectors.
	 *
	 *   *         (sam,rutgers)---(jane,rutgers)-----(bob,rutgers)   (sergei,rutgers)
	 * 	 * 	 *                           |                 |             |
	 * 	 * 	 *                           |                 |             |
	 * 	 * 	 *                      (kaitlin,rutgers)   (samir)----(aparna,rutgers)
	 * 	 * 	 *                           |                            |
	 * 	 * 	 *                           |                            |
	 * 	 * 	 *   (ming,penn state)----(nick,penn state)----(ricardo,penn state)
	 * 	 * 	 *                           |
	 * 	 * 	 *                           |
	 * 	 * 	 *                      (heather,penn state)
	 * 	 * 	 *
	 * 	 * 	 *
	 * 	 * 	 *                    (michele,cornell)----(rachel)
	 * 	 * 	 *                           |
	 * 	 * 	 *                           |
	 * 	 * 	 *      (rich,ucla)---(tom,ucla)
	 * 	 [jane, michele, aparna, nick, bob no, tom]
	 */
	public static ArrayList<String> connectors(Graph g) {


		ArrayList<String> result = new ArrayList<>();
		for(int i = 0; i < g.members.length; i++){
			ArrayList<String> people = new ArrayList<>();
			people = islandOfIndex(g, people, g.members[i].name);
			//people.remove(i);
			 ArrayList<String> checkRoutes = new ArrayList<>();
			 checkRoutes = avoidCurrent(g,checkRoutes, g.members[i].name, g.members[g.members[i].first.fnum].name);
			 if(checkRoutes.size() < people.size()){
			 	if(!result.contains(g.members[i].name)) {
					result.add(g.members[i].name);
				}
			 }
		}
		return result;
	}

	private static ArrayList<String> islandOfIndex(Graph g, ArrayList<String> people, String current){
		String firstFriendOfCurrent = g.members[g.members[g.map.get(current)].first.fnum].name;
		if(!people.contains(firstFriendOfCurrent)){
			people.add(firstFriendOfCurrent);
			people = islandOfIndex(g, people, firstFriendOfCurrent);
		}
		Friend node = g.members[g.map.get(current)].first;
		if(node.next != null){
			node = node.next;
			while(node != null){
				String otherFriendsOfCurrent = g.members[node.fnum].name;
				if(!people.contains(otherFriendsOfCurrent)){
					people.add(otherFriendsOfCurrent);
					people = islandOfIndex(g, people, otherFriendsOfCurrent);
				}
				node = node.next;
			}
		}
		return people;
	}

	private static ArrayList<String> avoidCurrent(Graph g,ArrayList<String> checkRoutes, String avoid, String current){
		if(current.equals(avoid)){
			return checkRoutes;
		}
		String firstFriendOfCurrent = g.members[g.members[g.map.get(current)].first.fnum].name;
		if(!checkRoutes.contains(firstFriendOfCurrent)){
			checkRoutes.add(firstFriendOfCurrent);
			checkRoutes = avoidCurrent(g, checkRoutes, avoid, firstFriendOfCurrent);
		}
		Friend node = g.members[g.map.get(current)].first;
		if(node.next != null){
			node = node.next;
			while(node != null){
				String otherFriendsOfCurrent = g.members[node.fnum].name;
				if(!checkRoutes.contains(otherFriendsOfCurrent)){
					checkRoutes.add(otherFriendsOfCurrent);
					checkRoutes = avoidCurrent(g, checkRoutes, avoid, otherFriendsOfCurrent);
				}
				node = node.next;
			}
		}
		return checkRoutes;
	}
}

