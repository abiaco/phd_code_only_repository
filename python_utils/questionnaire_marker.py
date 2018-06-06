import openpyxl as xl
pos_answers = {
	"Strongly Disagree" : 1,
	"Disagree" : 2,
	"Neutral" : 3,
	"Agree" : 4,
	"Strongly Agree" : 5,
	None : 3
}

neg_answers = {
	"Strongly Disagree" : 5,
	"Disagree" : 4,
	"Neutral" : 3,
	"Agree" : 2,
	"Strongly Agree" : 1,
	None : 3
}

positives = ["C", "E", "G", "I", "K", "M", "P", "Q", "S", "U", "W", "Y", "AA", "AC", "AE", "AG"]
negatives = ["D", "F", "H", "J", "L", "N", "O", "R", "T", "V", "X", "Z", "AB", "AD", "AF", "AH"]
spec = "AI"



f = xl.load_workbook("Immersion Questionnaire (Responses).xlsx")
first_sheet = f.get_sheet_names()[0]
worksheet = f.active
# get_sheet_by_name(first_sheet)

rows = worksheet.max_row

for i in range (3, rows + 1):
	print ("participant: " + worksheet["B" + str(i)].value)
	score = 0
	for letters in positives:
		score = score + pos_answers[worksheet[letters + str(i)].value]

	for letters in negatives:
		score = score + neg_answers[worksheet[letters + str(i)].value]

	score = score + worksheet[spec + str(i)].value

	print ("score: " + str(score))


print ("works")
