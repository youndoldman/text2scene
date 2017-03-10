#pragma once

#include "SemanticGraph.h"
#include "MetaData.h"

class ModelDatabase;
class TSScene;
class Model;


class SceneSemGraph : public SemanticGraph
{
public:
	SceneSemGraph();
	SceneSemGraph(const QString &s);
	SceneSemGraph(SceneSemGraph *sg);
	~SceneSemGraph();

	void loadGraph(const QString &filename);
	TSScene* covertToTSScene(unordered_map<string, Model*> &models);

	SceneSemGraph* getSubGraph(const vector<int> &nodeList, bool useContext = false);

	SemanticGraph* mergeWithTSG(SemanticGraph *tsg);  // update current UserSSG with textSSG for retrieval
	void mergeWithMatchedSSG(SceneSemGraph *matchedSg, std::map<int, int> &matchToNewUserSsgNodeMap); // update current USserSSG with retrieved subSSG


	int findParentNodeId(int modelId);
	int getNodeIdWithModelId(int modelId);

public:
	MetaScene m_metaScene;

	std::map<int, int> m_objectGraphNodeIdToModelSceneIdMap;	

	map<int, int> m_dbNodeToSubNodeMap;


private:
	int m_modelNum;
	QString m_fullFilename;
};

