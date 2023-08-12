package pairapi

import (
	businesslogic "example/gin/business-logic"
	"example/gin/core/models"
	dataaccess "example/gin/data-access"
	"strconv"

	"github.com/gin-gonic/gin"
)

var (
	PairBLL businesslogic.PairBLL
)

func Startup() {
	PairBLL = businesslogic.PairBLL{
		PairRepo: dataaccess.PairDAL{
			Source:       make(map[int]models.Pair),
			CurrentMaxId: 0,
		},
	}
}

func SetupRoutes(router *gin.Engine) {
	// simple example
	router.GET("/status", getStatus)

	// pair examples
	router.GET("/pairs", getPairs)
	router.GET("/pairs/:id", getPair)
	router.POST("/pairs", savePair)
	router.PUT("/pairs/:id", updatePair)
	router.DELETE("/pairs/:id", deletePair)
}

func getStatus(c *gin.Context) {
	c.JSON(200, "OK")
}

func getPairs(c *gin.Context) {
	allPairs := PairBLL.GetAll()
	c.JSON(200, allPairs)
}

func getPair(c *gin.Context) {
	id, err := strconv.Atoi(c.Param("id"))
	if err != nil {
		c.Status(400)
		return
	}

	pair, err := PairBLL.Get(id)
	if err != nil {
		c.Status(404)
		return
	}
	c.JSON(200, pair)
}

func savePair(c *gin.Context) {
	var pair models.Pair
	c.BindJSON(&pair)
	PairBLL.Save(pair)
	c.Status(201)
}

func updatePair(c *gin.Context) {
	id, err := strconv.Atoi(c.Param("id"))
	if err != nil {
		c.Status(400)
		return
	}

	var pair models.Pair
	c.BindJSON(&pair)

	err = PairBLL.Update(id, pair)
	if err != nil {
		c.Status(404)
		return
	}
	c.Status(204)
}

func deletePair(c *gin.Context) {
	id, err := strconv.Atoi(c.Param("id"))
	if err != nil {
		c.Status(400)
		return
	}

	err = PairBLL.Delete(id)
	if err != nil {
		c.Status(404)
		return
	}
	c.Status(204)
}
