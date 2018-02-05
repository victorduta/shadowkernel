import sys
import os
import re

profileData = []
markerData = []
excessFunctions = []
systemMap = []
objectMap = []
branch_max = 200
profile_weigth = 0
total_weigth = 0
pruned = 0
degree = 0.95

mapSize = 0
objectSize = 0
markerSize = 0

os.environ['KERNEL_VERSION'] = '4.4.17'
os.environ['KERNEL_DIR'] = 'llvm-kernel/' 
os.environ['KERNEL_MAP'] = 'System.map-'+os.environ['KERNEL_VERSION']
os.environ['KERNEL_DUMP'] = 'vmlinux_dump'
os.environ['PROFILING_DIR'] = os.environ['KERNEL_DIR']+'debug/profiling/'
os.environ['EXCESS_FILE'] = os.environ['PROFILING_DIR']+'excess_files'
os.environ['HEADER_FILE'] = os.environ['PROFILING_DIR']+'profiling_header.h'
os.environ['EXCLUDE_FILE'] = os.environ['PROFILING_DIR']+'exclude_header.h'



def mapProfileData(filename):
  inputFile = open(filename)
  for line in inputFile:
      elements=line.strip().split(" ")
      touple = (elements[0], int(elements[1]) , elements[2], int(elements[3]), elements[4])
      profileData.append(touple)

def mapMarkerData(filename):
  global markerSize
  inputFile = open(filename)
  for line in inputFile:
      elements=line.strip().split("||||")
      index_1 = elements[1].index("@")
      index_2 = elements[1].index(")")
      index_p1 = elements[0].index("@")
      index_p2 = elements[0].index("(")
      touple = (elements[1][index_1+19:index_2].strip() , elements[0][index_p1+1:index_p2], elements[2].strip())
      #print touple
      markerData.append(touple) 
  markerSize = len(markerData)
  inputFile.close()

def load_system_map():
  global mapSize
  inputFile = open('./'+os.environ['KERNEL_DIR']+os.environ['KERNEL_MAP'])
  for line in inputFile:
      elements = line.strip().split(" ")
      touple = (elements[0], elements[1], elements[2]) # Represents a system map entry ADDRESS TYPE NAME
      systemMap.append(touple)
  mapSize = len(systemMap)
  #print systemMap

def analyse_signature(call_signature):
    index= call_signature.find('callq 0x')
    if (index >= 0):
        return 'direct'
    else:
        return 'indirect'

# At this time this will only include call instructions
def load_object_map(instruction_filter):
  global objectSize
  inputFile = open('./'+os.environ['KERNEL_DIR']+os.environ['KERNEL_DUMP'])
  for line in inputFile:
      elements = line.replace("\t"," ").strip().split(":")
      if len(elements) >= 2:
         split_index = elements[1].find(instruction_filter)
         if split_index >= 0:
            call_signature_flat = elements[1][split_index:len(elements[1])]
            call_signature = re.sub("\s\s+" , " ", call_signature_flat)
            touple = (elements[0], analyse_signature(call_signature), call_signature)
            objectMap.append(touple)
  objectSize = len(objectMap)
  #print objectMap

def get_object_signature_recursive(callsite, inferior, superior):
  if inferior == superior:
      return objectMap[superior]
  middle = int((inferior+superior)/2)
  hexnumber = int(objectMap[middle][0], 16)
  address = int(callsite, 16)
  if hexnumber == address:
     return objectMap[middle]
  elif hexnumber < address:
        return get_object_signature_recursive(callsite, min(middle+1, objectSize-1), superior)
  else:
        return get_object_signature_recursive(callsite, inferior, middle)

def get_object_signature(callsite):
  return get_object_signature_recursive(callsite, 0, objectSize-1)

def get_object_marker_recursive(marker, inferior, superior):
   if inferior == superior:
      return markerData[superior]
   middle = int((inferior+superior)/2)
   entry = int(markerData[middle][0])
   searched = int(marker)
   if entry == searched:
      return markerData[middle]
   if entry < searched:
      return get_object_marker_recursive(marker, min(middle+1, markerSize-1), superior)
   else:
      return get_object_marker_recursive(marker, inferior, middle)

def get_object_marker(marker):
  return get_object_marker_recursive(marker, 0, markerSize-1)

def get_function_name(functionAddress):
  result = list(filter(lambda x: x[0] == functionAddress, systemMap))
  return list(map(lambda x: x[2], result))

def get_parent_recursively(functionAddress, inferior, superior):
   if inferior == superior:
      return systemMap[superior][0]
   middle = int((inferior+superior)/2)
   touple_1 = systemMap[middle]
   touple_2 = systemMap[min(middle+1, mapSize-1)]
   hexnumber_1 = int(touple_1[0], 16)
   hexnumber_2 = int(touple_2[0], 16)
   address = int(functionAddress, 16)
   if hexnumber_1 <= address and address < hexnumber_2:
      return touple_1[0]
   elif hexnumber_2 <= address:
      return get_parent_recursively(functionAddress, min(middle+1, mapSize-1), superior)
   elif address < hexnumber_1:
      return get_parent_recursively(functionAddress, inferior, middle)


def get_parent_function(functionAddress):
  result = get_parent_recursively(functionAddress, 0, len(systemMap) - 1)
  intermediary = list(filter(lambda x: x[0] == result, systemMap))
  return list(map(lambda x: x[2], intermediary))

def get_parent_address(functionAddress):
  result = get_parent_recursively(functionAddress, 0, len(systemMap))
  return result

def check_excess_branches():
    branchDict = {}
    for touple in profileData[:]:
        if touple[0] not in branchDict:
            branchDict[touple[0]] = 1
        else:
            branchDict[touple[0]] += 1
            if branchDict[touple[0]] == branch_max:
               excessFunctions.append(touple[0])
    return excessFunctions
    #print excessFunctions

def write_excess_branches(excessVector):
    file = open('./'+os.environ['EXCESS_FILE'], 'w')
    for address in excessVector:
        function = get_function_name(address)
        file.write(str(function).strip('[]')+"\n")
    file.close()

def translateToAnalysisTouple(touple):
    child = get_function_name(touple[0])
    parent = get_parent_function(touple[2])
    call_site = touple[2]
    
    if parent[0] == '__brk_limit': # TODO find a better way for this
       return (child, parent, call_site, touple[3], '', '')
    signature = get_object_signature(call_site)
    marker = get_object_marker(touple[4])
    if marker[1] not in parent:
       return (child, parent, call_site, touple[3], signature[1], 'unknown')
    return (child, parent, call_site, touple[3], signature[1], marker[2])

def checkAnalysisTouples(profiles):
    for elem in profiles:
        if not elem[0]:
           return False
        if not elem[1]:
           return False
    return True

def fitPercentage(percent, profiles):
    global profile_weigth
    global pruned
    profile_weigth = 0
    new_dict = {}
    for elem in profiles:
        profile_weigth += elem[3]
        new_dict[elem[0]] = elem[1] 
    aux = 0
    for elem in new_dict.values():
        aux += elem
   
    numPruned = int((1 - percent)*profile_weigth)
    print "Profile number: " + str(len(profiles))
    print "Weigth :" + str(profile_weigth)
    print "Total Weigth: " + str(aux)
    print "Missing weigth: "+ str(aux- profile_weigth)
    print "To prune : " + str(numPruned)
    start = 0
    eliminated = 0
    for elem in profiles[::-1]:
        if start+elem[3] <= numPruned:
           start += elem[3]
           eliminated += 1 
        else:
           break
    pruned = eliminated
    print "Pruned " + str(pruned)
     
##### Parser ########

def writeString(out, string):
    out.write("\""+string+"\"")
  
def writeInt(out, integer):
    out.write(str(integer))

def writeStruct(out, elements):
    out.write("{")
    out.write(" ")
    writeElement(out, elements[0])
    out.write(" ")
    for element in elements[1:]:
        out.write(",")
        writeElement(out, element)
        out.write(" ")
    out.write("}")

def writeElement(out, element):
    if type(element) == str:
       writeString(out, element)
       return
    if type(element) == int:
       writeInt(out, element)
       return
    if type(element) == list:
       writeStruct(out, element)
       return
    if type(element) == tuple:
       writeStruct(out, element)
       return

def writeStructDefinition(out):
     out.write('struct analysis_struct {\
       vector<string> function_aliases;\
       vector<string> parent_aliases;\
       string callsite_signature;\
       unsigned long long weigth;\
       string type;\
       string assembly_format;\
       \
       analysis_struct(vector<string> function_aliases,  vector<string> parent_aliases, string signature, unsigned long long weigth, string type, string assembly_format){\
           this->function_aliases = function_aliases;\
           this->parent_aliases = parent_aliases;\
           this->callsite_signature = signature;\
           this->weigth = weigth;\
           this->type = type;\
           this->assembly_format = assembly_format;\
       }\
     };')


def parserWrite(element):
    out = open('./'+os.environ['HEADER_FILE'], 'w')
    writeStructDefinition(out)
    out.write("\n")
    out.write("\n")
    out.write("vector<analysis_struct> profileVector = ")
    writeElement(out,element)
    out.write(";")
    out.close()

def excludeWrite(profiles):
    aux = []
    out = open('./'+os.environ['EXCLUDE_FILE'], 'w')
    out.write("vector<string> excludeVector = ")
    for elem in profiles:
        for bans in elem[0]:
            aux.append(bans)
    writeElement(out, aux)
    out.write(";")
    out.close()


def debugWrite(profile):
    out = open(sys.argv[3], 'w')
    for elem in profile:
        out.write(str(elem))
        out.write("\n")
    out.close()

def generateAnalysisVector(profiles):
    profiles.sort(key= lambda tup: -tup[3])
    fitPercentage(degree, profiles)
    analysisTouple = list(map(translateToAnalysisTouple, profiles))
    #isOk = checkAnalysisTouples(analysisTouple)
    #if isOk == False:
    #   print "Something went wrong when generating analysis touple"
    return analysisTouple
def main():
  mapProfileData(sys.argv[1])

  mapMarkerData(sys.argv[2])
  # Load system map to decypher function addresses
  load_system_map()

  # Load call map to determine type of call. Currently it is either director or indirect
  load_object_map('call')

  # Check function that have more than MAX_ENTRIES call sites
  #excessVector = check_excess_branches()
  #write_excess_branches(excessVector)

  
  analysisTouple = generateAnalysisVector(profileData)
  #print analysisTouple[0: (len(analysisTouple) - pruned)]
  parserWrite(analysisTouple[0: (len(analysisTouple) - pruned)])
  debugWrite(analysisTouple[0: (len(analysisTouple) - pruned)])
  excludeWrite(analysisTouple[0: (len(analysisTouple) - pruned)])

def test():
  mapMarkerData(sys.argv[2])

main()

#test()


