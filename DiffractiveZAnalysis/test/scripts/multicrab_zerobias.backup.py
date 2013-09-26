#
# RunA: 136035-138560, Castor
# RunB: 141956-150431, Castor
#
# 124009 - 147116: HLT_Mu9, 147116-150431: HLT_Mu15
#

[MULTICRAB]
cfg=crab.cfg

[ZeroBiasA]
CMSSW.pset = ZeroBiasPATTuplesDiffractiveZ_cfg.py
cmssw.output_file = ZeroBiasDiffractiveZPATTuple.root
cmssw.datasetpath = /ZeroBias/Run2010A-Apr21ReReco-v1/RECO
cmssw.pycfg_params = Run=A condition=Full 
cmssw.lumi_mask = CastorGood2010.json
cmssw.total_number_of_lumis = -1
cmssw.lumis_per_job = 20

[ZeroBiasB]
CMSSW.pset = ZeroBiasPATTuplesDiffractiveZ_cfg.py
cmssw.output_file = ZeroBiasDiffractiveZPATTuple.root
cmssw.datasetpath = /MinimumBias/Run2010B-Apr21ReReco-v1/RECO
cmssw.pycfg_params = Run=B condition=Full
cmssw.lumi_mask = CastorGood2010.json
cmssw.total_number_of_lumis = -1
cmssw.lumis_per_job = 7
