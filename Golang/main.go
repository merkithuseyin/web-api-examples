package main

import (
	pairapi "example/gin/api"

	"github.com/gin-gonic/gin"
)

func main() {
	router := gin.Default()

	pairapi.Startup()
	pairapi.SetupRoutes(router)

	router.Run("localhost:8080")
}
