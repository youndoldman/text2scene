#pragma once

#include "Headers.h"
#include "MetaData.h"

class SceneSemGraphManager;
class RelationModelManager;
class SemanticGraph;
class SceneSemGraph;

class SemGraphMatcher
{
public:
	SemGraphMatcher(SceneSemGraphManager *ssgManager, RelationModelManager *relManager);
	~SemGraphMatcher();

	void updateQuerySG(SemanticGraph *sg);

	vector<SceneSemGraph*> alignWithDatabaseSSGs(int targetMatchNum);

	SceneSemGraph* alignSSGWithDBSSG(SemanticGraph *querySSG, SceneSemGraph *dbSSG, double &matchingScore);

	void addEdgesToSubSSG(SceneSemGraph *matchedSubSSG, SceneSemGraph *dbSSG);
	void addGroupActNodesToSubSSG(SceneSemGraph *matchedSubSSG, SceneSemGraph *dbSSG);
	void addSynthNodeToSubSSG(SceneSemGraph *matchedSubSSG, SceneSemGraph *dbSSG);  // add unmatched nodes as synth nodes to Sub-SSG
	void addSuppParentNodesToSubSSG(SceneSemGraph *matchedSubSSG, SceneSemGraph *dbSSG);
	void addContextNodesToSubSSG(SceneSemGraph *matchedSubSSG, SceneSemGraph *dbSSG);  	// add high co-occur objects with probability

	MetaModel& retrieveForModelInstance(const QString catName);

	vector<int> findNonRepeatSSGs(const vector<pair<double, SceneSemGraph *>> &scoredDBSubSSGs, int targetNum);
	bool ssgContainsBlackListModel(SceneSemGraph *ssg);
	double computeGeometrySimilarity(SceneSemGraph *testSSG, SceneSemGraph *refSSG); // debug: simple compute similarity by matching node name

	void loadModelBlackList();
	bool isModelInBlackList(const QString &s);

private:
	SceneSemGraphManager *m_sceneSemGraphManager;
	RelationModelManager *m_relModelManager;
	SemanticGraph *m_querySSG;

	std::set<QString> m_modelBlackList;  // list of models that is irregular and need to be blacked
};

